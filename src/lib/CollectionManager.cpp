/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QDir>
#include <QFile>
#include <QUuid>
#include <QList>
#include <QLineF>
#include <QFileInfo>
#include <QDataStream>
#include <QHashIterator>
#include "ISI.h"
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
            ImportData* pImportDataHead = NULL;

            QHash<int, ImportData*> prevIds;
            prevIds.reserve(numComponents);

            for (int i=0; i < numComponents; i++)
            {
                bool isValid;
                in >> isValid;

                if (isValid)
                {
                    ImportData* pImportData = new ImportData;

                    ImportComponent(pImportData, in);

                    if (pImportData->pComponent != NULL)
                    {
                        pImportData->pNext = pImportDataHead;
                        pImportDataHead = pImportData;

                        prevIds.insert(pImportData->id, pImportData);
                    }
                    else
                    {
                        pImportData->pNext = NULL;

                        DeleteImportData(pImportData);
                    }
                }
            }

            RestorePositions(pImportDataHead, point);
            RestoreVisibility(pImportDataHead);
            RestoreParenting(pImportDataHead, prevIds);
            RestorePrivateData(pImportDataHead, prevIds);
            RestoreConnections(pImportDataHead, prevIds);

            CRefList crefs = m_pISI->NewCRefList();
            ImportData* pImportData = pImportDataHead;

            while (pImportData != NULL)
            {
                crefs.Append(pImportData->cref);
                pImportData = pImportData->pNext;
            }

            DeleteImportData(pImportDataHead);

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
       >> pImportData->inLayout
       >> pImportData->parentId
       >> pImportData->opacity
       >> pImportData->layerPos
       >> pImportData->layerBoundingRect
       >> pImportData->transformOrigin
       >> pImportData->rotation;


    qint32 privateDataCount;
    in  >> privateDataCount;

    if (privateDataCount > 0)
    {
        pImportData->privateData.reserve(privateDataCount);

        for (qint32 i=0; i < privateDataCount; i++)
        {
            qint32 key;
            in >> key;

            DRef dref = m_pISI->ImportData(in);

            pImportData->privateData.insert(key, dref);
        }
    }


    ImportConnections(pImportData, in);


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

            /*
             * Note: the instance should be released
             * after all processing is complete.
             */
        }

        pImportData->cref = cref;
        pImportData->pComponent = pComponent;
    }
}


void CollectionManager::ExportComponent(Component* pComponent, QDataStream& out)
{
    int type = pComponent->Type();
    out << m_pISI->GetUuid_C(type);

    out << (qint32)pComponent->m_pData->id;
    out << (qint32)pComponent->m_pData->layer;
    out << (bool)  pComponent->m_pData->visible;
    out << (bool)  pComponent->m_pData->inLayout;

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


    QHash<int, DRef> privateData;
    pComponent->Export(privateData);

    out << (qint32)privateData.count();

    QHashIterator<int, DRef> it(privateData);
    while (it.hasNext())
    {
        it.next();

        out << (qint32)it.key();

        DRef dref = it.value();
        m_pISI->ExportData(dref, out);
    }


    ExportConnections(pComponent, out);
}


void CollectionManager::ImportConnections(ImportData* pImportData, QDataStream& in)
{
    pImportData->pConnections = NULL;

    qint32 type;
    in >> type;

    while (type != ConnectionRecord::Type_Null)
    {
        ConnectionRecord* pConnection = new ConnectionRecord;

        in >> pConnection->id
           >> pConnection->localUuid
           >> pConnection->remoteUuid;

        pConnection->type = type;
        pConnection->pNext = pImportData->pConnections;
        pImportData->pConnections = pConnection;

        in >> type;
    }
}


void CollectionManager::ExportConnections(Component* pComponent, QDataStream& out)
{
    int numCallers = pComponent->IFace()->NumCallers();
    int numEmitters = pComponent->IFace()->NumEmitters();

    Connections* pConnections = &pComponent->m_pData->connections;

    for (int i=0; i < numEmitters; i++)
    {
        if (pConnections->EmitterConnected(i))
        {
            QString emitterUuid = pComponent->EmitterUuid(i);

            QHash<CRef, int> recipients = pConnections->EmitterRecipients(i);

            QHashIterator<CRef, int> it(recipients);
            while (it.hasNext())
            {
                it.next();

                CRef cref = it.key();

                Component* pComponent2 = cref.GetInstance();
                if (pComponent2 != NULL)
                {
                    QString receiverUuid = pComponent2->ReceiverUuid(it.value());

                    out << (qint32)ConnectionRecord::Type_Emitter;
                    out << (qint32)cref.Id();
                    out << emitterUuid;
                    out << receiverUuid;

                    cref.ReleaseInstance();
                }
            }
        }
    }


    for (int i=0; i < numCallers; i++)
    {
        if (pConnections->CallerConnected(i))
        {
            Tuple2<CRef, int> target = pConnections->GetCallerTarget(i);

            CRef cref = target.item1;

            Component* pComponent2 = cref.GetInstance();
            if (pComponent2 != NULL)
            {
                QString callerUuid = pComponent->CallerUuid(i);
                QString funcSetUuid = pComponent2->FuncSetUuid(target.item2);

                out << (qint32)ConnectionRecord::Type_Caller;
                out << (qint32)cref.Id();
                out << callerUuid;
                out << funcSetUuid;

                cref.ReleaseInstance();
            }
        }
    }


    /*
     * Mark the end of the connection records.
     */
    out << (qint32)ConnectionRecord::Type_Null;
}


void CollectionManager::RestorePositions(ImportData* pImportDataHead,
                                         const QPointF& point)
{
    QRectF boundingRect;

    ImportData* pImportData = pImportDataHead;

    while (pImportData != NULL)
    {
        if ((pImportData->layer == ViewLayerId_Main) &&
            (pImportData->visible))
        {
            boundingRect |= pImportData->layerBoundingRect;
        }

        pImportData = pImportData->pNext;
    }


    pImportData = pImportDataHead;

    while (pImportData != NULL)
    {
        if (pImportData->layer == ViewLayerId_Main)
        {
            QPointF oldCenter = boundingRect.center();
            QPointF delta = oldCenter - pImportData->layerPos;
            QPointF newPos = point - delta;

            pImportData->pComponent->SetPosition(newPos);
        }
        else
        {
            // TODO: what if the new window size is smaller or
            // different than the previous window size?
            pImportData->pComponent->SetPosition(pImportData->layerPos);
        }

        pImportData = pImportData->pNext;
    }
}


void CollectionManager::RestoreVisibility(ImportData* pImportData)
{
    while (pImportData != NULL)
    {
        if (pImportData->visible)
        {
            pImportData->pComponent->SetVisible(true);
        }

        pImportData = pImportData->pNext;
    }
}


void CollectionManager::RestoreParenting(ImportData* pImportData,
                                         const QHash<int, ImportData*>& prevIds)
{
    /*
     * Note: parenting must occur after
     * all the positions are set to avoid
     * incorrect placement.
     */

    while (pImportData != NULL)
    {
        int parentId = pImportData->parentId;

        if (parentId != -1)
        {
            ImportData* pParentData = prevIds.value(parentId, NULL);

            if (pParentData != NULL)
            {
                pImportData->pComponent->SetParent(pParentData->cref);

                if (pImportData->inLayout &&
                    m_pISI->IsLayout(pParentData->cref))
                {
                    pImportData->pComponent->m_pData->inLayout = true;
                }
            }
        }

        pImportData = pImportData->pNext;
    }
}


void CollectionManager::RestorePrivateData(ImportData* pImportData,
                                           const QHash<int, ImportData*>& prevIds)
{
    while (pImportData != NULL)
    {
        if (m_pISI->IsLayout(pImportData->cref))
        {
            /*
             * All layouts should store the previous ids
             * in slot 1 of their private exported data.
             */

            QHash<int, DRef>* pPrivateData = &pImportData->privateData;

            if (pPrivateData->contains(1))
            {
                DRef dref = pPrivateData->value(1);

                if (dref.Type() == BOI_STD_D(IntList))
                {
                    pPrivateData->remove(1);

                    QList<int>* pList = dref.GetWriteInstance<QList<int> >();

                    for (int i=0; i < pList->count(); i++)
                    {
                        ImportData* pData = prevIds.value(pList->at(i), NULL);

                        int newId = (pData != NULL) ? pData->cref.Id() : -1;

                        pList->replace(i, newId);
                    }

                    pPrivateData->insert(1, dref);
                }
            }
        }

        pImportData->pComponent->Import(pImportData->privateData);
        pImportData = pImportData->pNext;
    }
}


void CollectionManager::RestoreConnections(ImportData* pImportData,
                                           const QHash<int, ImportData*>& prevIds)
{
    while (pImportData != NULL)
    {
        ConnectionRecord* pConnection = pImportData->pConnections;

        while (pConnection != NULL)
        {
            ImportData* pTargetData = prevIds.value(pConnection->id, NULL);

            if (pTargetData != NULL)
            {
                if (pConnection->type == ConnectionRecord::Type_Caller)
                {
                    int caller = pImportData->pComponent->GetCaller(pConnection->localUuid);
                    int funcSet = pTargetData->pComponent->GetFuncSet(pConnection->remoteUuid);

                    if ((caller != -1) && (funcSet != -1))
                    {
                        pImportData->pComponent->ConnectToFuncSet(caller, pTargetData->cref, funcSet);
                    }
                }
                else if (pConnection->type == ConnectionRecord::Type_Emitter)
                {
                    int emitter = pImportData->pComponent->GetEmitter(pConnection->localUuid);
                    int receiver = pTargetData->pComponent->GetReceiver(pConnection->remoteUuid);

                    if ((emitter != -1) && (receiver != -1))
                    {
                        pImportData->pComponent->ConnectToReceiver(emitter, pTargetData->cref, receiver);
                    }
                }
            }

            pConnection = pConnection->pNext;
        }

        pImportData = pImportData->pNext;
    }
}


void CollectionManager::DeleteImportData(ImportData* pImportData)
{
    ImportData* pNext;

    while (pImportData != NULL)
    {
        pNext = pImportData->pNext;

        if (pImportData->pComponent != NULL)
        {
            pImportData->cref.ReleaseInstance();
            pImportData->cref.Reset();
        }

        ConnectionRecord* pRecord = pImportData->pConnections;
        ConnectionRecord* pNextRecord;

        while (pRecord != NULL)
        {
            pNextRecord = pRecord->pNext;

            delete pRecord;
            pRecord = pNextRecord;
        }

        delete pImportData;
        pImportData = pNext;
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

