/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "MenuItem.h"


namespace BOI {


MenuItem::MenuItem(const QString& title,
                   const QString& action,
                   const ActionArgs& args)
    : m_title(title),
      m_action(action),
      m_actionArgs(args),
      m_pPrev(NULL),
      m_pNext(NULL),
      m_pChild(NULL),
      m_pParent(NULL)
{
    m_actionArgs.SetAutoDelete(false);
}


void MenuItem::SetTitle(const QString& title)
{
    m_title = title;
}


void MenuItem::SetAction(const QString& action)
{
    m_action = action;
}


void MenuItem::SetActionArgs(const ActionArgs& args)
{
    m_actionArgs = args;
    m_actionArgs.SetAutoDelete(false);
}


MenuItem* MenuItem::SetPrev(const QString& title,
                            const QString& action,
                            const ActionArgs& args)
{
    MenuItem* pItem = new MenuItem(title, action, args);
    pItem->m_pParent = m_pParent;
    pItem->m_pNext = this;

    if (m_pPrev != NULL)
    {
        m_pPrev->m_pNext = pItem;
        pItem->m_pPrev = m_pPrev;
    }
    else if ((m_pParent != NULL) &&
             (m_pParent->m_pChild == this))
    {
        /*
         * If this is the first child of the parent
         * then make the new item the first child.
         */

        m_pParent->m_pChild = pItem;
    }

    m_pPrev = pItem;

    return pItem;
}


MenuItem* MenuItem::SetNext(const QString& title,
                            const QString& action,
                            const ActionArgs& args)
{
    MenuItem* pItem = new MenuItem(title, action, args);
    pItem->m_pParent = m_pParent;
    pItem->m_pPrev = this;

    if (m_pNext != NULL)
    {
        m_pNext->m_pPrev = pItem;
        pItem->m_pNext = m_pNext;
    }

    m_pNext = pItem;

    return pItem;
}


MenuItem* MenuItem::SetChild(const QString& title,
                             const QString& action,
                             const ActionArgs& args)
{
    MenuItem* pItem = new MenuItem(title, action, args);
    pItem->m_pParent = this;

    if (m_pChild != NULL)
    {
        m_pChild->m_pPrev = pItem;
        pItem->m_pNext = m_pChild;
    }

    m_pChild = pItem;

    return pItem;
}


MenuItem* MenuItem::AddChild(const QString& title,
                             const QString& action,
                             const ActionArgs& args)
{
    MenuItem* pItem = new MenuItem(title, action, args);
    pItem->m_pParent = this;

    if (m_pChild == NULL)
    {
        m_pChild = pItem;
    }
    else
    {
        MenuItem* pLastChild = m_pChild->LastSibling();

        pLastChild->m_pNext = pItem;
        pItem->m_pPrev = pLastChild;
    }

    return pItem;
}


MenuItem* MenuItem::Delete()
{
    /*
     * Delete the current item and return
     * a pointer to the next item if there
     * is one. If this item has children,
     * then all the children will be deleted
     * as well.
     */

    MenuItem* pNext = m_pNext;

    if (m_pChild != NULL)
    {
        MenuItem* pChild = m_pChild;

        while (pChild != NULL)
        {
            pChild = pChild->Delete();
        }
    }

    if (m_pNext != NULL)
    {
        m_pNext->m_pPrev = m_pPrev;
    }

    if (m_pPrev != NULL)
    {
        m_pPrev->m_pNext = m_pNext;
    }

    if ((m_pParent != NULL) &&
        (m_pParent->m_pChild == this))
    {
        m_pParent->m_pChild = m_pNext;
    }

    delete this;
    return pNext;
}


MenuItem* MenuItem::FindChild(const QString& title)
{
    MenuItem* pItem = m_pChild;

    while (pItem != NULL)
    {
        if (pItem->m_title == title)
        {
            break;
        }

        pItem = pItem->m_pNext;
    }

    return pItem;
}


MenuItem* MenuItem::FindSibling(const QString& title, bool inclusive)
{
    MenuItem* pItem;

    if (inclusive && (m_title == title))
    {
        return this;
    }

    if (m_pParent != NULL)
    {
        pItem = m_pParent->m_pChild;
    }
    else
    {
        pItem = m_pPrev;

        while (pItem != NULL)
        {
            if (pItem->m_title == title)
            {
                return pItem;
            }

            pItem = pItem->m_pPrev;
        }

        pItem = m_pNext;
    }


    while (pItem != NULL)
    {
        if ((pItem != this) &&
            (pItem->m_title == title))
        {
            break;
        }

        pItem = pItem->m_pNext;
    }

    return pItem;
}


MenuItem* MenuItem::FindNextSibling(const QString& title)
{
    MenuItem* pItem = m_pNext;

    while (pItem != NULL)
    {
        if (pItem->m_title == title)
        {
            break;
        }

        pItem = pItem->m_pNext;
    }

    return pItem;
}


MenuItem* MenuItem::LastSibling()
{
    MenuItem* pItem = this;

    while (pItem->m_pNext != NULL)
    {
        pItem = pItem->m_pNext;
    }

    return pItem;
}


} // namespace BOI

