/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QString>
#include "ASI.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "Events/TouchEvent.h"
#include "Actions/SetNumberAction.h"


namespace BOI {


SetNumberAction::SetNumberAction()
    : Action(BOI_STD_A(SetNumber))
{
    m_xStep = 20;
    m_yStep = 20;
}


ActionCommand SetNumberAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    m_numTouchStreams = 0;
    m_minValue = 1;
    m_maxValue = 1;
    m_minDelta = 1;
    m_maxDelta = 1;
    m_receiver = 0;
    m_value = 1;

    if (pArgs == NULL)
    {
        return BOI_AC_STOP;
    }

    m_activeComponent = pSI->ActiveComponent();

    if (!m_activeComponent.IsValid())
    {
        return BOI_AC_STOP;
    }

    if (pArgs->Contains("Receiver"))
    {
        QString uuid = pArgs->Value<QString>("Receiver");
        m_receiver = pSI->GetReceiver(m_activeComponent, uuid);

        if (m_receiver == -1)
        {
            return BOI_AC_STOP;
        }
    }

    if (pArgs->Contains("Value"))
    {
        m_value = pArgs->Value<double>("Value");

        SetValue(pSI, m_value);
    }

    if (pArgs->Contains("MinValue"))
    {
        m_minValue = pArgs->Value<double>("MinValue");
    }

    if (pArgs->Contains("MaxValue"))
    {
        m_maxValue = pArgs->Value<double>("MaxValue");
    }

    if (pArgs->Contains("MinDelta"))
    {
        m_minDelta = pArgs->Value<double>("MinDelta");
    }

    if (pArgs->Contains("MaxDelta"))
    {
        m_maxDelta = pArgs->Value<double>("MaxDelta");
    }

    if (pArgs->Contains("Interactive"))
    {
        bool interactive = pArgs->Value<bool>("Interactive");

        if (!interactive)
        {
            return BOI_AC_STOP;
        }
    }

    return BOI_AC_CONTINUE;
}


void SetNumberAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_activeComponent.Reset();
}


bool SetNumberAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand SetNumberAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    double x = pEvent->x;
    double y = pEvent->y;

    if (eventType == TouchEvent::Type_Press)
    {
        m_xPrev = x;
        m_yPrev = y;

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Move) 
    {
        double xChange = x - m_xPrev;

        if ((xChange > 0) && (xChange >= m_xStep))
        {
            double newValue = m_value + m_maxDelta;

            if (newValue <= m_maxValue)
            {
                m_value = newValue;
                SetValue(pSI, m_value);
            }

            m_xPrev = x;
        }
        else if ((xChange < 0) && (xChange <= -m_xStep))
        {
            double newValue = m_value - m_maxDelta;

            if (newValue >= m_minValue)
            {
                m_value = newValue;
                SetValue(pSI, m_value);
            }

            m_xPrev = x;
        }


        double yChange = y - m_yPrev;

        if ((yChange > 0) && (yChange >= m_yStep))
        {
            double newValue = m_value - m_minDelta;

            if (newValue >= m_minValue)
            {
                m_value = newValue;
                SetValue(pSI, m_value);
            }

            m_yPrev = y;
        }
        else if ((yChange < 0) && (yChange <= -m_yStep))
        {
            double newValue = m_value + m_minDelta;

            if (newValue <= m_maxValue)
            {
                m_value = newValue;
                SetValue(pSI, m_value);
            }

            m_yPrev = y;
        }
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


void SetNumberAction::SetValue(ASI* pSI, double value)
{
    DRef dref = pSI->NewData(BOI_STD_D(Double));
    *dref.GetWriteInstance<double>() = value;

    pSI->EmitTo(m_activeComponent, m_receiver, dref);
}


} // namespace BOI

