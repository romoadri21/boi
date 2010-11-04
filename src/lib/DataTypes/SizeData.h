/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SIZEDATA_H
#define __BOI_SIZEDATA_H


#include <QSizeF>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class SizeData
    : public Data
{
    public:
        SizeData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QSizeF m_size;
};


inline SizeData::SizeData()
    : Data(BOI_STD_D(Size)),
      m_size()
{
}


inline void* SizeData::Instance()
{
    return &m_size;
}


inline void SizeData::CopyInstance(Data* pData)
{
    ((SizeData*)pData)->m_size = m_size;
}


inline void SizeData::Import(QDataStream& in)
{
    in >> m_size;
}


inline void SizeData::Export(QDataStream& out)
{
    out << m_size;
}


} // namespace BOI


#endif //__BOI_SIZEDATA_H

