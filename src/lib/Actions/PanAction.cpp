/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QVariant>
#include "ASI.h"
#include "StateId.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/PanAction.h"


namespace BOI {


PanAction::PanAction()
    : Action(BOI_STD_A(Pan))
{
}


ActionCommand PanAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    pSI->SetCursor(Qt::OpenHandCursor);

    return BOI_AC_CONTINUE;
}


void PanAction::Stop(ASI* pSI)
{
    pSI->UnsetCursor();
}


bool PanAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand PanAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (m_numTouchStreams == 1)
    {
        if (!m_scrollerActive)
        {
            if (eventType == TouchEvent::Type_Move) 
            {
                qreal x = pEvent->x;
                qreal y = pEvent->y;

                m_touchVelocity.AddTouch(x, y);

                if (m_innerViewRect.contains(x, y))
                {
                    x -= m_startX;
                    y -= m_startY;

                    x /= m_scaleFactor;
                    y /= m_scaleFactor;

                    pSI->TranslateView(x, y);

                    m_startX = pEvent->x;
                    m_startY = pEvent->y;
                }
                else
                {
                    qreal args[3];
                    args[0] = x - m_startX;
                    args[1] = y - m_startY;
                    args[2] = m_scrollVelocity / m_scaleFactor;

                    m_scrollerId = pSI->AppendViewTransformer(BOI_VTID(Scroll), 1, args);

                    if (m_scrollerId != -1)
                    {
                        m_scrollerActive = true;
                    }
                }
            }
            else if (eventType == TouchEvent::Type_Release) 
            {
                if ((m_touchVelocity.InstantaneousMagnitude() >= 1200.0) && // TODO: move hardcoded value into state
                    (m_touchVelocity.Magnitude() >= m_kineticScrollActivateVelocity))
                {
                    QPointF direction = m_touchVelocity.Direction();

                    qreal args[4];
                    args[0] = direction.x();
                    args[1] = direction.y();
                    args[2] = m_touchVelocity.Magnitude() / m_scaleFactor;
                    args[3] = m_kineticScrollAcceleration / m_scaleFactor;
                    pSI->AppendViewTransformer(BOI_VTID(KineticScroll), 1, args);
                }

                pSI->SetCursor(Qt::OpenHandCursor);
                m_numTouchStreams--;
            }
        }
        else // (m_scrollerActive == true)
        {
            if (eventType == TouchEvent::Type_Move) 
            {
                qreal x = pEvent->x;
                qreal y = pEvent->y;

                if (m_innerViewRect.contains(x, y))
                {
                    m_scrollerActive = false;
                    pSI->StopViewTransformer(m_scrollerId);

                    m_startX = pEvent->x;
                    m_startY = pEvent->y;
                }
                else
                {
                    qreal args[3];
                    args[0] = x - m_startX;
                    args[1] = y - m_startY;
                    args[2] = m_scrollVelocity / m_scaleFactor;

                    if (!pSI->UpdateViewTransformer(m_scrollerId, args))
                    {
                        m_scrollerActive = false;
                    }
                }
            }
            else if (eventType == TouchEvent::Type_Release) 
            {
                m_scrollerActive = false;

                pSI->StopViewTransformer(m_scrollerId);
                pSI->SetCursor(Qt::OpenHandCursor);

                m_numTouchStreams--;
            }
        }
    }
    else
    {
        if (eventType == TouchEvent::Type_Press)
        {
            QVariant var;

            pSI->GetState(StateId_InnerViewRect, var);
            m_innerViewRect = var.toRectF();

            pSI->GetState(StateId_ScrollVelocity, var);
            m_scrollVelocity = var.toDouble();

            pSI->GetState(StateId_KineticScrollAcceleration, var);
            m_kineticScrollAcceleration = var.toDouble();

            pSI->GetState(StateId_KineticScrollActivateVelocity, var);
            m_kineticScrollActivateVelocity = var.toDouble();


            pSI->SetCursor(Qt::ClosedHandCursor);

            m_startX = pEvent->x;
            m_startY = pEvent->y;

            m_scaleFactor = pSI->ViewScaleFactor();

            m_scrollerActive = false;

            m_touchVelocity.Reset();

            m_numTouchStreams++;
        }
    }

    return BOI_AC_CONTINUE;
}


} // namespace BOI

