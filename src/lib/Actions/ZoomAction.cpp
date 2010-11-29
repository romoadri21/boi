/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/ZoomAction.h"


namespace BOI {


ZoomAction::ZoomAction()
    : Action(BOI_STD_A(Zoom)),
      m_sensitivity(0.035f)
{
}


ActionCommand ZoomAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    pSI->SetCursor(Qt::SizeVerCursor);

    return BOI_AC_CONTINUE;
}


void ZoomAction::Stop(ASI* pSI)
{
    pSI->UnsetCursor();
}


bool ZoomAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand ZoomAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (m_numTouchStreams == 1)
    {
        if (eventType == TouchEvent::Type_Move) 
        {
            int y = pEvent->y;
            float scaleFactor = 1.0f;

            if (y <= m_startY)     scaleFactor += m_sensitivity;
            else if (y > m_startY) scaleFactor -= m_sensitivity;

            pSI->ScaleView(scaleFactor);
            pSI->AlignLayerToView(m_layerPoint, m_viewPoint);

            m_startY = y;
        }
        else if (eventType == TouchEvent::Type_Release) 
        {
            m_numTouchStreams--;
        }
    }
    else
    {
        if (eventType == TouchEvent::Type_Press)
        {
            m_startY = pEvent->y;

            m_viewPoint.setX(pEvent->x);
            m_viewPoint.setY(pEvent->y);

            m_layerPoint = pSI->MapFromViewToLayer(m_viewPoint);

            m_numTouchStreams++;
        }
    }

    return BOI_AC_CONTINUE;
}


} // namespace BOI

