/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_PENDATA_H
#define __BOI_PENDATA_H


#include <QPen>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class PenData
    : public Data
{
    public:
        PenData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QPen m_pen;
};


inline PenData::PenData()
    : Data(BOI_STD_D(Pen)),
      m_pen()
{
}


inline void* PenData::Instance()
{
    return &m_pen;
}


inline void PenData::CopyInstance(Data* pData)
{
    ((PenData*)pData)->m_pen = m_pen;
}


inline void PenData::Import(QDataStream& in)
{
    in >> m_pen;
}


inline void PenData::Export(QDataStream& out)
{
    out << m_pen;
}


} // namespace BOI


#endif //__BOI_PENDATA_H

