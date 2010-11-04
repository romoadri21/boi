/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STRINGDATA_H
#define __BOI_STRINGDATA_H


#include <QString>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class StringData
    : public Data
{
    public:
        StringData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QString m_string;
};


inline StringData::StringData()
    : Data(BOI_STD_D(String)),
      m_string()
{
}


inline void* StringData::Instance()
{
    return &m_string;
}


inline void StringData::CopyInstance(Data* pData)
{
    ((StringData*)pData)->m_string = m_string;
}


inline void StringData::Import(QDataStream& in)
{
    in >> m_string;
}


inline void StringData::Export(QDataStream& out)
{
    out << m_string;
}


} // namespace BOI


#endif //__BOI_STRINGDATA_H

