/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_INTDATA_H
#define __BOI_INTDATA_H


#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class IntData
    : public Data
{
    public:
        IntData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        int m_int;
};


inline IntData::IntData()
    : Data(BOI_STD_D(Int))
{
}


inline void* IntData::Instance()
{
    return &m_int;
}


inline void IntData::CopyInstance(Data* pData)
{
    ((IntData*)pData)->m_int = m_int;
}


inline void IntData::Import(QDataStream& in)
{
    qint64 value;
    in >> value;
    m_int = value;
}


inline void IntData::Export(QDataStream& out)
{
    out << (qint64)m_int;
}


} // namespace BOI


#endif //__BOI_INTDATA_H

