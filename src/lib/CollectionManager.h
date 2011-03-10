/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COLLECTIONMANAGER_H
#define __BOI_COLLECTIONMANAGER_H


#include <QHash>
#include <QRectF>
#include <QPointF>
#include <QString>
#include "CRef.h"
#include "DRef.h"
#include "CRefList.h"


class QDataStream;


namespace BOI {


class ISI;
class Component;


class CollectionManager
{
    public:
        CollectionManager();
        ~CollectionManager();

        void SetISI(ISI* pISI);

        bool Initialize(const QString& basePath);

        QString CreateCollection(CRefList crefs,
                                 const QString& uuid=QString());

        CRefList OpenCollection(const QString& uuid,
                                const QPointF& point);

        bool CollectionExists(const QString& uuid);
        bool DeleteCollection(const QString& uuid);

    private:
        typedef struct ConnectionRecord
        {
            enum
            {
                Type_Null = 0,
                Type_Caller,
                Type_Emitter
            };

            qint32 id;
            qint32 type;

            QString localUuid;
            QString remoteUuid;

            ConnectionRecord* pNext;
        } ConnectionRecord;

        typedef struct ImportData
        {
            ImportData()
                : pConnections(NULL),
                  pComponent(NULL),
                  pNext(NULL) {}

            QString uuid;

            qint32 id;
            qint32 layer;
            qint32 parentId;

            QPointF layerPos;
            QPointF transformOrigin;

            QRectF layerBoundingRect;

            ConnectionRecord* pConnections;

            QHash<int, DRef> privateData;

            qreal rotation;
            qreal opacity;

            bool visible;
            bool inLayout;

            CRef cref;
            Component* pComponent;

            ImportData* pNext;
        } ImportData;

    private:
        void ImportComponent(ImportData* pImportData, QDataStream& in);
        void ExportComponent(Component* pComponent, QDataStream& out);

        void ImportConnections(ImportData* pImportData, QDataStream& in);
        void ExportConnections(Component* pComponent, QDataStream& out);

        void RestorePositions(ImportData* pImportDataHead,
                              const QPointF& centerPoint);

        void RestoreVisibility(ImportData* pImportDataHead);

        void RestoreParenting(ImportData* pImportDataHead,
                              const QHash<int, ImportData*>& prevIds);

        void RestorePrivateData(ImportData* pImportDataHead,
                                const QHash<int, ImportData*>& prevIds);

        void RestoreConnections(ImportData* pImportDataHead,
                                const QHash<int, ImportData*>& prevIds);

        void DeleteImportData(ImportData* pImportData);

        QString GetPath(const QString& uuid);

    private:
        ISI* m_pISI;

        QString m_basePath;
};


} // namespace BOI


#endif //__BOI_COLLECTIONMANAGER_H

