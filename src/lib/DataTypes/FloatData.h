/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_FLOATDATA_H
#define __BOI_FLOATDATA_H


#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class FloatData
    : public Data
{
    public:
        FloatData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        float m_float;
};


inline FloatData::FloatData()
    : Data(BOI_STD_D(Float))
{
}


inline void* FloatData::Instance()
{
    return &m_float;
}


inline void FloatData::CopyInstance(Data* pData)
{
    ((FloatData*)pData)->m_float = m_float;
}


inline void FloatData::Import(QDataStream& in)
{
    in >> m_float;
}


inline void FloatData::Export(QDataStream& out)
{
    out << m_float;
}


} // namespace BOI


#endif //__BOI_FLOATDATA_H

