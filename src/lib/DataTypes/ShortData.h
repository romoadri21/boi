/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SHORTDATA_H
#define __BOI_SHORTDATA_H


#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class ShortData
    : public Data
{
    public:
        ShortData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        qint16 m_short;
};


inline ShortData::ShortData()
    : Data(BOI_STD_D(Short))
{
}


inline void* ShortData::Instance()
{
    return &m_short;
}


inline void ShortData::CopyInstance(Data* pData)
{
    ((ShortData*)pData)->m_short = m_short;
}


inline void ShortData::Import(QDataStream& in)
{
    in >> m_short;
}


inline void ShortData::Export(QDataStream& out)
{
    out << m_short;
}


} // namespace BOI


#endif //__BOI_SHORTDATA_H

