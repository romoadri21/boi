/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include <QVariant>
#include "math.h"
#include "ASI.h"
#include "MenuSet.h"
#include "ActionArgs.h"
#include "SceneLayerId.h"
#include "StandardMenus.h"
#include "StandardActions.h"
#include "Events/KeyEvent.h"
#include "Events/TouchEvent.h"
#include "StandardComponents.h"
#include "Actions/XGestureAction.h"


namespace BOI {


XGestureAction::XGestureAction()
    : Action(BOI_STD_A(XGesture)),
      m_radius(15.0),
      m_menuComponent(),
      m_xgestureComponent()
{
}


ActionCommand XGestureAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    if (!m_menuComponent.IsValid())
    {
        m_menuComponent = pSI->NewComponent(BOI_STD_C(Menu),
                                            SceneLayerId_System);

        m_menuFuncSet = pSI->GetFuncSet(m_menuComponent,
                                        "{1c412c24-6f31-4138-b1f6-b3f4d662fb67}");
        if (m_menuFuncSet == -1)
        {
            /*
             * TODO: this will cause Start() to always try to get a new menu component
             * if it is Reset() because it will always be invalid. Since this will not
             * change throughout the life of the application, make this a bool or
             * something so that it only runs once.
             */
            m_menuComponent.Reset();
        }
    }

    if (!m_xgestureComponent.IsValid())
    {
        m_xgestureComponent = pSI->NewComponent(BOI_STD_C(XGesture),
                                                SceneLayerId_System);

        pSI->SetSelectable(m_xgestureComponent, false);
    }

    return BOI_AC_CONTINUE;
}


bool XGestureAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand XGestureAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    ActionCommand command = BOI_AC_CONTINUE;
    int eventType = pEvent->type;

    if (m_numTouchStreams == 1)
    {
        if (eventType == TouchEvent::Type_Move) 
        {
            double a = pEvent->x - m_x1;
            double b = pEvent->y - m_y1;

            double length = a*a + b*b;
            length = sqrt(length);

            if (length >= m_radius)
            {
                int quadrant = Quadrant(a, b);
                if (quadrant == 1)
                {
                    command = BOI_AC_REPLACE(BOI_STD_A(Zoom), NULL);
                }
                else if (quadrant == 3)
                {
                    command = BOI_AC_REPLACE(BOI_STD_A(Pan), NULL);
                }
                else if (quadrant == 4)
                {
                    command = BOI_AC_REPLACE(BOI_STD_A(Move), NULL);
                }
                else
                {
                    command = BOI_AC_STOP;
                }

                pSI->SetVisible(m_xgestureComponent, false);
            }
        }
        else if (eventType == TouchEvent::Type_Release) 
        {
            pSI->SetVisible(m_xgestureComponent, false);

            QPointF centerPoint(pEvent->x, pEvent->y);

            if (ShowMenu(pSI, centerPoint))
            {
                QVariant var;
                var.setValue(m_menuComponent);

                ActionArgs* pArgs = new ActionArgs;
                pArgs->Set("CRef", var);

                command = BOI_AC_REPLACE(BOI_STD_A(Hider), pArgs);
            }

            m_numTouchStreams--;
        }
    }
    else
    {
        if (eventType == TouchEvent::Type_Press)
        {
            m_x1 = pEvent->x;
            m_y1 = pEvent->y;

            QPointF pos(m_x1, m_y1);
            pSI->CenterComponentOn(m_xgestureComponent, pos);
            pSI->SetVisible(m_xgestureComponent, true);

            m_numTouchStreams++;
        }
    }

    return command;
}


int XGestureAction::Quadrant(double x, double y)
{
    int quadrant = 0;

    if (x >= 0)
    {
        if (y >= 0)
        {
            if (x >= y) quadrant = 1;
            else quadrant = 2;
        }
        else
        {
            y *= -1;

            if (x >= y) quadrant = 1;
            else quadrant = 4;
        }
    }
    else
    {
        if (y >= 0)
        {
            x *= -1;

            if (x >= y) quadrant = 3;
            else quadrant = 2;
        }
        else
        {
            x *= -1;
            y *= -1;

            if (x >= y) quadrant = 3;
            else quadrant = 4;
        }
    }

    return quadrant;
}


bool XGestureAction::ShowMenu(ASI* pSI, const QPointF& centerPoint)
{
    if (!m_menuComponent.IsValid()) return false;

    MenuSet menuSet;
    CRef activeComponent;

    int sceneLayerIds = (SceneLayerId_All ^ SceneLayerId_System);
    CRefList crefList = pSI->ComponentsAtViewPoint(centerPoint.toPoint(), sceneLayerIds);
    if (crefList.Count() > 0)
    {
        activeComponent = crefList.Value(0);

        pSI->GetMenus(activeComponent, menuSet);
        pSI->SetActiveComponent(activeComponent);
    }
    else
    {
        /*
         * If no component was clicked then set
         * the active component to an invalid
         * component.
         */
        pSI->SetActiveComponent(activeComponent);
    }


    Component* pComponent = m_menuComponent.GetInstance();
    if (pComponent != NULL)
    {
        DRef dref = pSI->NewData(BOI_STD_D(MenuSet));

        const MenuItem* pMenuItem = pSI->GetMenu(BOI_UUID_M(Root));
        dref.GetWriteInstance<MenuSet>()->Append(pMenuItem);
        pSI->CallFunc(pComponent, m_menuFuncSet, 1, dref);

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
                pSI->CallFunc(pComponent, m_menuFuncSet, 3, dref);

                /*
                 * Set the primary context menu to the
                 * custom component menu.
                 */
                *dref.GetWriteInstance<MenuSet>() = menuSet;
                pSI->CallFunc(pComponent, m_menuFuncSet, 2, dref);
            }
            else
            {
                /*
                 * Set the secondary context menu to nothing.
                 */
                pSI->CallFunc(pComponent, m_menuFuncSet, 3, dref);

                /*
                 * Set the primary context menu to the
                 * general/standard component menu.
                 */
                const MenuItem* pMenuItem = pSI->GetMenu(BOI_UUID_M(Component));
                dref.GetWriteInstance<MenuSet>()->Append(pMenuItem);
                pSI->CallFunc(pComponent, m_menuFuncSet, 2, dref);
            }
        }
        else // (!activeComponent.IsValid())
        {
            /*
             * Set the primary and secondary context
             * menus to nothing.
             */
            pSI->CallFunc(pComponent, m_menuFuncSet, 2, dref);
            pSI->CallFunc(pComponent, m_menuFuncSet, 3, dref);
        }

        /*
         * Set the center point.
         */
        dref = pSI->NewData(BOI_STD_D(Point));
        *dref.GetWriteInstance<QPointF>() = centerPoint;
        pSI->CallFunc(pComponent, m_menuFuncSet, 4, dref);

        /*
         * Generate the menu.
         */
        pSI->CallFunc(pComponent, m_menuFuncSet, 5, dref);

        pSI->StackOnTop(m_menuComponent);
        pSI->SetVisible(m_menuComponent, true);

        m_menuComponent.ReleaseInstance();
        return true;
    }

    return false;
}


ActionCommand XGestureAction::HandleKeyEvent(ASI* pSI, KeyEvent* pEvent)
{
    Q_UNUSED(pSI);

    ActionCommand command = BOI_AC_CONTINUE;

    if (pEvent->type == KeyEvent::Type_Press) 
    {
        /*
         * TODO: If this is a space then activate a
         * tag related action which allows the user
         * to do a search by typing in tag words?
         */
    }

    return command;
}


void XGestureAction::Destroy()
{
    m_menuComponent.Reset();
    m_xgestureComponent.Reset();
}


} // namespace BOI

