/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPointF>
#include "ASI.h"
#include "MenuSet.h"
#include "ActionArgs.h"
#include "ViewLayerId.h"
#include "StandardMenus.h"
#include "StandardActions.h"
#include "StandardComponents.h"
#include "Actions/MenuAction.h"


namespace BOI {


MenuAction::MenuAction()
    : Action(BOI_STD_A(Menu))
{
}


ActionCommand MenuAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    if (!m_cref.IsValid())
    {
        m_cref = pSI->NewComponent(BOI_STD_C(Menu), ViewLayerId_Null);
        m_funcSet = pSI->GetFuncSet(m_cref, "{1c412c24-6f31-4138-b1f6-b3f4d662fb67}");

        if (m_funcSet == -1)
        {
            /*
             * TODO: this will cause Start() to always try to get a new menu component
             * if it is Reset() because it will always be invalid. Since this will not
             * change throughout the life of the application, make this a bool or
             * something so that it only runs once.
             */
            m_cref.DestroyInstance();
            m_cref.Reset();
        }
    }


    QPointF centerPoint;

    if ((pArgs != NULL) &&
        (pArgs->Contains("CenterPoint")))
    {
        centerPoint = pArgs->Value<QPointF>("CenterPoint");
    }
    else
    {
        return BOI_AC_STOP;
    }


    if (m_cref.IsValid())
    {
        if (ShowMenu(pSI, centerPoint))
        {
            return BOI_AC_CONTINUE;
        }
    }

    return BOI_AC_STOP;
}


void MenuAction::Stop(ASI* pSI)
{
    if (m_cref.IsValid())
    {
        pSI->MoveToLayer(m_cref, ViewLayerId_Null);
        pSI->SetVisible(m_cref, false);
    }
}


void MenuAction::Destroy()
{
    if (m_cref.IsValid())
    {
        m_cref.DestroyInstance();
    }

    m_cref.Reset();
}


bool MenuAction::ShowMenu(ASI* pSI, const QPointF& centerPoint)
{
    if (!m_cref.IsValid()) return false;

    MenuSet menuSet;
    CRef activeComponent = pSI->ActiveComponent();

    if (activeComponent.IsValid())
    {
        pSI->GetMenus(activeComponent, menuSet);
    }


    Component* pComponent = m_cref.GetInstance();
    if (pComponent != NULL)
    {
        DRef dref = pSI->NewData(BOI_STD_D(MenuSet));

        const MenuItem* pMenuItem = pSI->GetMenu(BOI_UUID_M(Root));
        dref.GetWriteInstance<MenuSet>()->Append(pMenuItem);
        pSI->CallFunc(pComponent, m_funcSet, 1, dref);

        dref.GetWriteInstance<MenuSet>()->Clear();

        if (activeComponent.IsValid())
        {
            if (!menuSet.IsEmpty())
            {
                /*
                 * Set the secondary context menu to the
                 * general/standard component menu.
                 */
                const MenuItem* pMenuItem = pSI->GetMenu(BOI_UUID_M(Component));
                dref.GetWriteInstance<MenuSet>()->Append(pMenuItem);
                pSI->CallFunc(pComponent, m_funcSet, 3, dref);

                /*
                 * Set the primary context menu to the
                 * custom component menu.
                 */
                *dref.GetWriteInstance<MenuSet>() = menuSet;
                pSI->CallFunc(pComponent, m_funcSet, 2, dref);
            }
            else
            {
                /*
                 * Set the secondary context menu to nothing.
                 */
                pSI->CallFunc(pComponent, m_funcSet, 3, dref);

                /*
                 * Set the primary context menu to the
                 * general/standard component menu.
                 */
                const MenuItem* pMenuItem = pSI->GetMenu(BOI_UUID_M(Component));
                dref.GetWriteInstance<MenuSet>()->Append(pMenuItem);
                pSI->CallFunc(pComponent, m_funcSet, 2, dref);
            }
        }
        else // (!activeComponent.IsValid())
        {
            /*
             * Set the primary and secondary context
             * menus to nothing.
             */
            pSI->CallFunc(pComponent, m_funcSet, 2, dref);
            pSI->CallFunc(pComponent, m_funcSet, 3, dref);
        }

        /*
         * Set the center point.
         */
        dref = pSI->NewData(BOI_STD_D(Point));
        *dref.GetWriteInstance<QPointF>() = centerPoint;
        pSI->CallFunc(pComponent, m_funcSet, 4, dref);

        /*
         * Generate the menu.
         */
        pSI->CallFunc(pComponent, m_funcSet, 5, dref);

        pSI->MoveToLayer(m_cref, ViewLayerId_System);
        pSI->StackOnTop(m_cref);
        pSI->SetVisible(m_cref, true);

        m_cref.ReleaseInstance();
        return true;
    }

    return false;
}


} // namespace BOI

