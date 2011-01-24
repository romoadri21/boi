/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COLLECTIONMANAGER_H
#define __BOI_COLLECTIONMANAGER_H


#include <QRectF>
#include <QPointF>
#include <QString>
#include "CRef.h"
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
        typedef struct
        {
            QString uuid;

            qint32 id;
            qint32 layer;
            qint32 dataSize;
            qint32 parentId;

            QPointF layerPos;
            QPointF transformOrigin;

            QRectF layerBoundingRect;

            qreal rotation;
            qreal opacity;

            bool visible;
            CRef cref;
        } ImportData;

    private:
        void ImportComponent(ImportData* pImportData, QDataStream& in);
        void ExportComponent(Component* pComponent, QDataStream& out);

        QString GetPath(const QString& uuid);

    private:
        ISI* m_pISI;

        QString m_basePath;
};


} // namespace BOI


#endif //__BOI_COLLECTIONMANAGER_H

