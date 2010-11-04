/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MENUITEM_H
#define __BOI_MENUITEM_H


#include <QString>
#include "BOILib.h"
#include "ActionArgs.h"


namespace BOI {


class BOI_LIB_EXPORT MenuItem
{
    public:
        MenuItem(const QString& title,
                 const QString& action = QString(),
                 const ActionArgs& args = ActionArgs());

        void SetTitle(const QString& title);
        const QString& Title() const;

        void SetAction(const QString& action);
        const QString& GetAction() const;

        void SetActionArgs(const ActionArgs& args);
        const ActionArgs* GetActionArgs() const;

        MenuItem* SetPrev(const QString& title,
                          const QString& action = QString(),
                          const ActionArgs& args = ActionArgs());
        MenuItem* Prev();
        const MenuItem* Prev() const;

        MenuItem* SetNext(const QString& title,
                          const QString& action = QString(),
                          const ActionArgs& args = ActionArgs());
        MenuItem* Next();
        const MenuItem* Next() const;

        MenuItem* SetChild(const QString& title,
                           const QString& action = QString(),
                           const ActionArgs& args = ActionArgs());

        MenuItem* AddChild(const QString& title,
                           const QString& action = QString(),
                           const ActionArgs& args = ActionArgs());

        MenuItem* Child();
        const MenuItem* Child() const;

        MenuItem* Parent();
        const MenuItem* Parent() const;

        MenuItem* Delete();

        MenuItem* FindChild(const QString& title);

        MenuItem* FindSibling(const QString& title,
                              bool inclusive = true);

        MenuItem* FindNextSibling(const QString& title);

        MenuItem* LastSibling();

    private:
        MenuItem();

    private:
        QString m_title;
        QString m_action;

        ActionArgs m_actionArgs;

        MenuItem* m_pPrev;
        MenuItem* m_pNext;
        MenuItem* m_pChild;
        MenuItem* m_pParent;
};


inline const QString& MenuItem::Title() const
{
    return m_title;
}


inline const QString& MenuItem::GetAction() const
{
    return m_action;
}


inline const ActionArgs* MenuItem::GetActionArgs() const
{
    return (m_actionArgs.NumArgs() > 0) ? &m_actionArgs : NULL;
}


inline MenuItem* MenuItem::Prev()
{
    return m_pPrev;
}


inline const MenuItem* MenuItem::Prev() const
{
    return m_pPrev;
}


inline MenuItem* MenuItem::Next()
{
    return m_pNext;
}


inline const MenuItem* MenuItem::Next() const
{
    return m_pNext;
}


inline MenuItem* MenuItem::Child()
{
    return m_pChild;
}


inline const MenuItem* MenuItem::Child() const
{
    return m_pChild;
}


inline MenuItem* MenuItem::Parent()
{
    return m_pParent;
}


inline const MenuItem* MenuItem::Parent() const
{
    return m_pParent;
}


} // namespace BOI


#endif //__BOI_MENUITEM_H

