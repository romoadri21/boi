/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/ResetViewScaleAction.h"


namespace BOI {


ResetViewScaleAction::ResetViewScaleAction()
    : Action(BOI_STD_A(ResetViewScale))
{
}


ActionCommand ResetViewScaleAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    return BOI_AC_CONTINUE;
}


bool ResetViewScaleAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand ResetViewScaleAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        QPoint point(pEvent->x, pEvent->y);

        QPointF centerPoint = pSI->MapFromViewToLayer(point);

        pSI->ResetViewTransform();
        pSI->CenterViewOn(centerPoint);

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Release)
    {
        m_numTouchStreams--;
    }

    return BOI_AC_STOP;
}


} // namespace BOI

