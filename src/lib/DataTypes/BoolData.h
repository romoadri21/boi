/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_BOOLDATA_H
#define __BOI_BOOLDATA_H


#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class BoolData
    : public Data
{
    public:
        BoolData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        bool m_boolean;
};


inline BoolData::BoolData()
    : Data(BOI_STD_D(Bool))
{
}


inline void* BoolData::Instance()
{
    return &m_boolean;
}


inline void BoolData::CopyInstance(Data* pData)
{
    ((BoolData*)pData)->m_boolean = m_boolean;
}


inline void BoolData::Import(QDataStream& in)
{
    in >> m_boolean;
}


inline void BoolData::Export(QDataStream& out)
{
    out << m_boolean;
}


} // namespace BOI


#endif //__BOI_BOOLDATA_H

