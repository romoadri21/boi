/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/MoveAction.h"


namespace BOI {


MoveAction::MoveAction()
    : Action(BOI_STD_A(Move))
{
}


ActionCommand MoveAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    pSI->SetCursor(Qt::OpenHandCursor);

    return BOI_AC_CONTINUE;
}


void MoveAction::Stop(ASI* pSI)
{
    m_cref.Reset();

    pSI->UnsetCursor();
}


bool MoveAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand MoveAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        pSI->SetCursor(Qt::ClosedHandCursor);

        QPoint point(pEvent->x, pEvent->y);

        int sceneLayers = SceneLayerId_Main |
                          SceneLayerId_Overlay |
                          SceneLayerId_Underlay;

        CRefList crefs = pSI->ComponentsAtViewPoint(point, sceneLayers);

        if (crefs.Count() > 0)
        {
            m_cref = crefs.Value(0);

            QPointF origin = pSI->MapToParent(m_cref, QPointF(0, 0));
            QPointF touchPoint = pSI->MapFromViewToParent(m_cref, point);

            m_xDelta = touchPoint.x() - origin.x();
            m_yDelta = touchPoint.y() - origin.y();
        }

        m_numTouchStreams++;
    }
    if (eventType == TouchEvent::Type_Move) 
    {
        if (m_cref.IsValid())
        {
            QPointF point = pSI->MapFromViewToParent(m_cref, QPoint(pEvent->x, pEvent->y));
            point -= QPointF(m_xDelta, m_yDelta);

            pSI->SetPosition(m_cref, point);
        }
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        m_cref.Reset();

        pSI->SetCursor(Qt::OpenHandCursor);
        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


} // namespace BOI

