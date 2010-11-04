/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CHARDATA_H
#define __BOI_CHARDATA_H


#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class CharData
    : public Data
{
    public:
        CharData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        quint8 m_char;
};


inline CharData::CharData()
    : Data(BOI_STD_D(Char))
{
}


inline void* CharData::Instance()
{
    return &m_char;
}


inline void CharData::CopyInstance(Data* pData)
{
    ((CharData*)pData)->m_char = m_char;
}


inline void CharData::Import(QDataStream& in)
{
    in >> m_char;
}


inline void CharData::Export(QDataStream& out)
{
    out << m_char;
}


} // namespace BOI


#endif //__BOI_CHARDATA_H

