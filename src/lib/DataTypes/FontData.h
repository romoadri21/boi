/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_FONTDATA_H
#define __BOI_FONTDATA_H


#include <QFont>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class FontData
    : public Data
{
    public:
        FontData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QFont m_font;
};


inline FontData::FontData()
    : Data(BOI_STD_D(Font)),
      m_font()
{
}


inline void* FontData::Instance()
{
    return &m_font;
}


inline void FontData::CopyInstance(Data* pData)
{
    ((FontData*)pData)->m_font = m_font;
}


inline void FontData::Import(QDataStream& in)
{
    in >> m_font;
}


inline void FontData::Export(QDataStream& out)
{
    out << m_font;
}


} // namespace BOI


#endif //__BOI_FONTDATA_H

