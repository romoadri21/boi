/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_POINTDATA_H
#define __BOI_POINTDATA_H


#include <QPointF>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class PointData
    : public Data
{
    public:
        PointData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QPointF m_point;
};


inline PointData::PointData()
    : Data(BOI_STD_D(Point)),
      m_point()
{
}


inline void* PointData::Instance()
{
    return &m_point;
}


inline void PointData::CopyInstance(Data* pData)
{
    ((PointData*)pData)->m_point = m_point;
}


inline void PointData::Import(QDataStream& in)
{
    in >> m_point;
}


inline void PointData::Export(QDataStream& out)
{
    out << m_point;
}


} // namespace BOI


#endif //__BOI_POINTDATA_H

