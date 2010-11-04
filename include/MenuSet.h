/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MENUSET_H
#define __BOI_MENUSET_H


#define BOI_MAX_MENUSET_MENUS 11


namespace BOI {


class MenuItem;


class MenuSet
{
    public:
        MenuSet();

        void Clear();
        bool IsEmpty() const;

        int NumMenus() const;

        bool Append(const MenuSet& menuSet);
        bool Append(const MenuItem* pRootItem);

        const MenuItem* MenuAt(int index) const;

    private:
        int m_numMenus;
        const MenuItem* m_menus[BOI_MAX_MENUSET_MENUS];
};


inline MenuSet::MenuSet()
    : m_numMenus(0)
{
}


inline void MenuSet::Clear()
{
    m_numMenus = 0;
}


inline bool MenuSet::IsEmpty() const
{
    return (m_numMenus == 0);
}


inline int MenuSet::NumMenus() const
{
    return m_numMenus;
}


inline bool MenuSet::Append(const MenuSet& menuSet)
{
    for (int i=0; i < menuSet.NumMenus(); i++)
    {
        if (m_numMenus < BOI_MAX_MENUSET_MENUS)
        {
            m_menus[m_numMenus] = menuSet.MenuAt(i);
            m_numMenus++;
        }
        else return false;
    }

    return true;
}


inline bool MenuSet::Append(const MenuItem* pRootItem)
{
    if (m_numMenus < BOI_MAX_MENUSET_MENUS)
    {
        m_menus[m_numMenus] = pRootItem;
        m_numMenus++;
        return true;
    }

    return false;
}


inline const MenuItem* MenuSet::MenuAt(int index) const
{
    return (index < m_numMenus) ?  m_menus[index] : NULL;
}


} // namespace BOI


#endif //__BOI_MENUSET_H

