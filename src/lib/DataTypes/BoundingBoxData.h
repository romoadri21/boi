/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_BOUNDINGBOXDATA_H
#define __BOI_BOUNDINGBOXDATA_H


#include <QRectF>
#include <QPointF>
#include "Data.h"
#include "Utilities/Tuple.h"
#include "StandardDataTypes.h"


namespace BOI {


class BoundingBoxData
    : public Data
{
    typedef Tuple2<QRectF, QPointF> DataType;

    public:
        BoundingBoxData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        DataType m_boundingBox;
};


inline BoundingBoxData::BoundingBoxData()
    : Data(BOI_STD_D(BoundingBox)),
      m_boundingBox()
{
}


inline void* BoundingBoxData::Instance()
{
    return &m_boundingBox;
}


inline void BoundingBoxData::CopyInstance(Data* pData)
{
    ((BoundingBoxData*)pData)->m_boundingBox = m_boundingBox;
}


inline void BoundingBoxData::Import(QDataStream& in)
{
    in >> m_boundingBox.item1;
    in >> m_boundingBox.item2;
}


inline void BoundingBoxData::Export(QDataStream& out)
{
    out << m_boundingBox.item1;
    out << m_boundingBox.item2;
}


} // namespace BOI


#endif //__BOI_BOUNDINGBOXDATA_H

