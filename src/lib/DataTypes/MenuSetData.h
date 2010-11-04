/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MENUSETDATA_H
#define __BOI_MENUSETDATA_H


#include "Data.h"
#include "MenuSet.h"
#include "StandardDataTypes.h"


namespace BOI {


class MenuSetData
    : public Data
{
    public:
        MenuSetData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        MenuSet m_menuSet;
};


inline MenuSetData::MenuSetData()
    : Data(BOI_STD_D(MenuSet)),
      m_menuSet()
{
}


inline void* MenuSetData::Instance()
{
    return &m_menuSet;
}


inline void MenuSetData::CopyInstance(Data* pData)
{
    ((MenuSetData*)pData)->m_menuSet = m_menuSet;
}


inline void MenuSetData::Import(QDataStream& in)
{
    Q_UNUSED(in);
    m_menuSet.Clear();
}


inline void MenuSetData::Export(QDataStream& out)
{
    Q_UNUSED(out);
}


} // namespace BOI


#endif //__BOI_MENUSETDATA_H

