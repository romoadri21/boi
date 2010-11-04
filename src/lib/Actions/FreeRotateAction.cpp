/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/FreeRotateAction.h"


namespace BOI {


FreeRotateAction::FreeRotateAction()
    : Action(BOI_STD_A(FreeRotate)),
      m_sensitivity(0.5)
{
}


ActionCommand FreeRotateAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    m_cref = pSI->ActiveComponent();

    if (!m_cref.IsValid())
    {
        return BOI_AC_STOP;
    }

    return BOI_AC_CONTINUE;
}


void FreeRotateAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_cref.Reset();
}


bool FreeRotateAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand FreeRotateAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        m_initialY = pEvent->y;

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Move) 
    {
        if (!m_cref.IsDestroyed())
        {
            qreal rotation = pEvent->y - m_initialY;
            rotation *= m_sensitivity;

            pSI->Rotate(m_cref, rotation);

            m_initialY = pEvent->y;
        }
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


} // namespace BOI

