/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QDir>
#include <QFile>
#include <QUuid>
#include <QHash>
#include <QLineF>
#include <QFileInfo>
#include <QDataStream>
#include <QHashIterator>
#include "ISI.h"
#include "DRef.h"
#include "Component.h"
#include "GraphicsItem.h"
#include "ViewLayerId.h"
#include "ComponentData.h"
#include "CollectionManager.h"


#define BOI_COLLECTION_MAGIC_NUMBER    0xA749BC02
#define BOI_COLLECTION_VERSION_1       0x00000001
#define BOI_COLLECTION_CURRENT_VERSION BOI_COLLECTION_VERSION_1


namespace BOI {


CollectionManager::CollectionManager()
    : m_basePath()
{
}


CollectionManager::~CollectionManager()
{
}


void CollectionManager::SetISI(ISI* pISI)
{
    m_pISI = pISI;
}


bool CollectionManager::Initialize(const QString& basePath)
{
    m_basePath = basePath;

    QDir baseDir(m_basePath);
    if (!baseDir.exists())
    {
        QString path = baseDir.absolutePath();
        if (!baseDir.mkpath(path))
        {
            return false;
        }
    }

    return true;
}


QString CollectionManager::CreateCollection(CRefList crefs,
                                            const QString& uuidIn)
{
    int numCRefs = crefs.Count();
    if (numCRefs == 0) return QString();

    QString uuid;
    QString filePath;

    QUuid tempUuid(uuidIn);

    if (!tempUuid.isNull())
    {
        uuid = tempUuid.toString();
    }
    else
    {
        uuid = QUuid::createUuid().toString();
    }


    filePath = GetPath(uuid);

    while (QFile::exists(filePath))
    {
        uuid = QUuid::createUuid().toString();
        filePath = GetPath(uuid);
    }

    QDir dir = QFileInfo(filePath).dir();
    if (!dir.exists())
    {
        dir.mkpath(dir.absolutePath());
    }


    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return QString();

    QDataStream out(&file);

    out << (quint32)BOI_COLLECTION_MAGIC_NUMBER;
    out << (qint32) BOI_COLLECTION_CURRENT_VERSION;

    out.setVersion(QDataStream::Qt_4_6);

    out << (qint32)numCRefs;

    for (int i=numCRefs-1; i >= 0; i--)
    {
        CRef cref = crefs.Value(i);

        Component* pComponent = cref.GetInstance();
        if (pComponent != NULL)
        {
            out << (bool)true;

            ExportComponent(pComponent, out);
            cref.ReleaseInstance();
        }
        else
        {
            out << (bool)false;
        }
    }

    file.close();

    return uuid;
}


CRefList CollectionManager::OpenCollection(const QString& uuid,
                                           const QPointF& point)
{
    if (uuid.length() < 4) return CRefList();

    QString filePath = GetPath(uuid);

    if (!QFile::exists(filePath)) return CRefList();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return CRefList();

    QDataStream in(&file);

    quint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != BOI_COLLECTION_MAGIC_NUMBER) return CRefList();

    qint32 version;
    in >> version;

    if (version == BOI_COLLECTION_VERSION_1)
    {
        in.setVersion(QDataStream::Qt_4_6);

        qint32 numComponents;
        in >> numComponents;

        if (numComponents > 0)
        {
            ImportData* pImportData = new ImportData[numComponents];

            QHash<int, ImportData*> prevIds;
            prevIds.reserve(numComponents);

            QRectF boundingRect;

            for (int i=0; i < numComponents; i++)
            {
                bool isValid;
                in >> isValid;

                if (isValid)
                {
                    ImportData* pData = &pImportData[i];

                    ImportComponent(pData, in);

                    if (pData->cref.IsValid())
                    {
                        prevIds.insert(pData->id, pData);

                        if ((pData->layer == ViewLayerId_Main) &&
                            (pData->visible))
                        {
                            boundingRect |= pData->layerBoundingRect;
                        }
                    }
                }
            }


            CRefList crefs = m_pISI->NewCRefList();

            for (int i=0; i < numComponents; i++)
            {
                ImportData* pData = &pImportData[i];

                if (pData->cref.IsValid())
                {
                    Component* pComponent = pData->cref.GetInstance();
                    if (pComponent != NULL)
                    {
                        if (pData->layer == ViewLayerId_Main)
                        {
                            QPointF oldCenter = boundingRect.center();
                            QPointF delta = oldCenter - pData->layerPos;
                            QPointF newPos = point - delta;

                            pComponent->SetPosition(newPos);
                        }
                        else
                        {
                            // TODO: what if the new window size is smaller or
                            // different than the previous window size?
                            pComponent->SetPosition(pData->layerPos);
                        }

                        if (pData->visible)
                        {
                            pComponent->SetVisible(true);
                        }

                        crefs.Append(pData->cref);

                        pData->cref.ReleaseInstance();
                    }
                }
            }

            for (int i=0; i < numComponents; i++)
            {
                ImportData* pData = &pImportData[i];

                if (pData->cref.IsValid())
                {
                    Component* pComponent = pData->cref.GetInstance();
                    if (pComponent != NULL)
                    {
                        int parentId = pData->parentId;

                        if (parentId != -1)
                        {
                            ImportData* pParentData = prevIds.value(parentId, NULL);

                            if (pParentData != NULL)
                            {
                                /*
                                 * Note: parenting must occur after
                                 * all the positions are set to avoid
                                 * incorrect placement.
                                 */
                                pComponent->SetParent(pParentData->cref);
                            }
                        }

                        pData->cref.ReleaseInstance();
                    }
                }
            }

            delete[] pImportData;
            return crefs;
        }
    }

    return CRefList();
}


void CollectionManager::ImportComponent(ImportData* pImportData, QDataStream& in)
{
    /*
     * Read in all the values from the data stream
     * regardless of whether or not the system supports
     * the particular type of component so that the
     * QDataStream is at the correct location for
     * components stored after this one in the stream.
     */

    in >> pImportData->uuid
       >> pImportData->id
       >> pImportData->layer
       >> pImportData->visible
       >> pImportData->parentId
       >> pImportData->opacity
       >> pImportData->layerPos
       >> pImportData->layerBoundingRect
       >> pImportData->transformOrigin
       >> pImportData->rotation
       >> pImportData->dataSize;

    QHash<int, DRef> data;
    data.reserve(pImportData->dataSize);

    for (qint32 i=0; i < pImportData->dataSize; i++)
    {
        qint32 key;
        in >> key;

        DRef dref = m_pISI->ImportData(in);

        data.insert(key, dref);
    }


    int type = m_pISI->ConvertUuid_C(pImportData->uuid);
    if (type != -1)
    {
        CRef cref = m_pISI->NewComponent(type, (ViewLayerId)pImportData->layer);

        Component* pComponent = cref.GetInstance();
        if (pComponent != NULL)
        {
            pComponent->SetOpacity(pImportData->opacity);
            pComponent->SetRotation(pImportData->rotation);
            pComponent->SetTransformOrigin(pImportData->transformOrigin);
            pComponent->Import(data);

            cref.ReleaseInstance();
        }

        pImportData->cref = cref;
    }
}


void CollectionManager::ExportComponent(Component* pComponent, QDataStream& out)
{
    int type = pComponent->Type();
    out << m_pISI->GetUuid_C(type);

    out << (qint32)pComponent->m_pData->id;
    out << (qint32)pComponent->m_pData->layer;
    out << (bool)  pComponent->m_pData->visible;

    GraphicsItem* pItem = &pComponent->m_pData->graphicsItem;
    GraphicsItem* pParent = (GraphicsItem*)pItem->parentItem();

    int parentId = ((pParent != NULL) && (pParent->parentItem() != NULL)) ?
                   (pParent->GetComponent()->m_pData->id) :
                   (-1);

    out << (qint32)parentId;

    out << pItem->opacity();
    out << pItem->LayerPos();
    out << pItem->LayerBoundingRect();
    out << pItem->transformOriginPoint();

    /*
     * Export the rotation relative to the scene.
     */
    QLineF itemLine(pItem->mapToScene(0, 0),
                    pItem->mapToScene(1, 0));
    QLineF sceneLine(0, 0, 1, 0);
    out << itemLine.angleTo(sceneLine);


    QHash<int, DRef> data;
    pComponent->Export(data);

    out << (qint32)data.size();

    QHashIterator<int, DRef> it(data);
    while (it.hasNext())
    {
        it.next();

        out << (qint32)it.key();

        DRef dref = it.value();
        m_pISI->ExportData(dref, out);
    }
}


bool CollectionManager::CollectionExists(const QString& uuid)
{
    if (uuid.length() < 4) return false;

    return QFile::exists(GetPath(uuid));
}


bool CollectionManager::DeleteCollection(const QString& uuid)
{
    if (uuid.length() < 4) return false;

    return QFile::remove(GetPath(uuid));
}


QString CollectionManager::GetPath(const QString& uuid)
{
    QString path = m_basePath + '/' + uuid.at(1) +
                                '/' + uuid.at(2) +
                                '/' + uuid.at(3);
    return path + '/' + uuid;
}


} // namespace BOI

