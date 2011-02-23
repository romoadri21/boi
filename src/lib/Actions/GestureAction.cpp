/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "math.h"
#include "ASI.h"
#include "ActionArgs.h"
#include "ViewLayerId.h"
#include "ComponentFlag.h"
#include "StandardActions.h"
#include "Events/KeyEvent.h"
#include "Events/TouchEvent.h"
#include "StandardComponents.h"
#include "Actions/GestureAction.h"


namespace BOI {


GestureAction::GestureAction()
    : Action(BOI_STD_A(Gesture)),
      m_radius(15.0),
      m_xgestureComponent()
{
}


ActionCommand GestureAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    if (!m_xgestureComponent.IsValid())
    {
        m_xgestureComponent = pSI->NewComponent(BOI_STD_C(XGesture),
                                                ViewLayerId_Null);

        pSI->SetVisible(m_xgestureComponent, true);
        pSI->SetFlag(m_xgestureComponent, ComponentFlag_IsSelectable, false);
    }

    return BOI_AC_CONTINUE;
}


bool GestureAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand GestureAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    ActionCommand command = BOI_AC_CONTINUE;
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        m_x1 = pEvent->x;
        m_y1 = pEvent->y;

        QPointF pos(m_x1, m_y1);
        pSI->CenterComponentOn(m_xgestureComponent, pos);
        pSI->MoveToLayer(m_xgestureComponent, ViewLayerId_System);

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Move) 
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

            pSI->MoveToLayer(m_xgestureComponent, ViewLayerId_Null);
        }
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        pSI->MoveToLayer(m_xgestureComponent, ViewLayerId_Null);

        QPointF centerPoint(pEvent->x, pEvent->y);

        int viewLayerIds = (ViewLayerId_All ^ ViewLayerId_System);
        CRefList crefList = pSI->ComponentsAtViewPoint(centerPoint.toPoint(), viewLayerIds);
        if (crefList.Count() > 0)
        {
            pSI->SetActiveComponent(crefList.Value(0));
        }
        else
        {
            CRef cref;
            pSI->SetActiveComponent(cref);
        }

        ActionArgs* pArgs = new ActionArgs;
        pArgs->Set("CenterPoint", centerPoint);

        command = BOI_AC_REPLACE(BOI_STD_A(Menu), pArgs);

        m_numTouchStreams--;
    }

    return command;
}


int GestureAction::Quadrant(double x, double y)
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


ActionCommand GestureAction::HandleKeyEvent(ASI* pSI, KeyEvent* pEvent)
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


void GestureAction::Destroy()
{
    if (m_xgestureComponent.IsValid())
    {
        m_xgestureComponent.DestroyInstance();
    }

    m_xgestureComponent.Reset();
}


} // namespace BOI

