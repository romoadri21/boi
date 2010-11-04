/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_DOUBLEDATA_H
#define __BOI_DOUBLEDATA_H


#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class DoubleData
    : public Data
{
    public:
        DoubleData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        double m_double;
};


inline DoubleData::DoubleData()
    : Data(BOI_STD_D(Double))
{
}


inline void* DoubleData::Instance()
{
    return &m_double;
}


inline void DoubleData::CopyInstance(Data* pData)
{
    ((DoubleData*)pData)->m_double = m_double;
}


inline void DoubleData::Import(QDataStream& in)
{
    in >> m_double;
}


inline void DoubleData::Export(QDataStream& out)
{
    out << m_double;
}


} // namespace BOI


#endif //__BOI_DOUBLEDATA_H

