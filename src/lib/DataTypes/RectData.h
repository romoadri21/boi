/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_RECTDATA_H
#define __BOI_RECTDATA_H


#include <QRectF>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class RectData
    : public Data
{
    public:
        RectData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QRectF m_rect;
};


inline RectData::RectData()
    : Data(BOI_STD_D(Rect)),
      m_rect()
{
}


inline void* RectData::Instance()
{
    return &m_rect;
}


inline void RectData::CopyInstance(Data* pData)
{
    ((RectData*)pData)->m_rect = m_rect;
}


inline void RectData::Import(QDataStream& in)
{
    in >> m_rect;
}


inline void RectData::Export(QDataStream& out)
{
    out << m_rect;
}


} // namespace BOI


#endif //__BOI_RECTDATA_H

