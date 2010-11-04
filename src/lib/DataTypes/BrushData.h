/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_BRUSHDATA_H
#define __BOI_BRUSHDATA_H


#include <QBrush>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class BrushData
    : public Data
{
    public:
        BrushData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QBrush m_brush;
};


inline BrushData::BrushData()
    : Data(BOI_STD_D(Brush)),
      m_brush()
{
}


inline void* BrushData::Instance()
{
    return &m_brush;
}


inline void BrushData::CopyInstance(Data* pData)
{
    ((BrushData*)pData)->m_brush = m_brush;
}


inline void BrushData::Import(QDataStream& in)
{
    in >> m_brush;
}


inline void BrushData::Export(QDataStream& out)
{
    out << m_brush;
}


} // namespace BOI


#endif //__BOI_BRUSHDATA_H

