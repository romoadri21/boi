/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COLORDATA_H
#define __BOI_COLORDATA_H


#include <QColor>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class ColorData
    : public Data
{
    public:
        ColorData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QColor m_color;
};


inline ColorData::ColorData()
    : Data(BOI_STD_D(Color)),
      m_color()
{
}


inline void* ColorData::Instance()
{
    return &m_color;
}


inline void ColorData::CopyInstance(Data* pData)
{
    ((ColorData*)pData)->m_color = m_color;
}


inline void ColorData::Import(QDataStream& in)
{
    in >> m_color;
}


inline void ColorData::Export(QDataStream& out)
{
    out << m_color;
}


} // namespace BOI


#endif //__BOI_COLORDATA_H

