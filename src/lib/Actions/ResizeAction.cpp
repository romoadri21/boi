/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QStringList>
#include <QPointF>
#include <QSizeF>
#include "ASI.h"
#include "Events/KeyEvent.h"
#include "Events/TouchEvent.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "Actions/ResizeAction.h"


namespace BOI {


ResizeAction::ResizeAction()
    : Action(BOI_STD_A(Resize))
{
    m_args.SetAutoDelete(false);
    m_args.SetPtr("TextPointer", &m_text);
    m_args.SetPtr("ErrorCode", &m_errorCode);
    m_args.Set("ClearOnNextPress", false);
}


ActionCommand ResizeAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    m_doSuspend = false;
    m_numTouchStreams = 0;

    m_axis = Axis_XY;

    if ((pArgs != NULL) &&
        (pArgs->Contains("Axis")))
    {
        QString axis = pArgs->Value<QString>("Axis");
        axis = axis.toLower();

        if (axis == "x")
        {
            m_axis = Axis_X;
        }
        else if (axis == "y")
        {
            m_axis = Axis_Y;
        }
    }


    m_cref = pSI->ActiveComponent();

    if (m_cref.IsValid())
    {
        m_setSizeReceiver = pSI->GetReceiver(m_cref, "{fb33af5a-942a-4e9d-814e-87d67228bc26}");
        m_updateSizeReceiver = pSI->GetReceiver(m_cref, "{b06ff327-be49-4617-9391-f192b64b82f5}");

        if ((m_setSizeReceiver != -1) &&
            (m_updateSizeReceiver != -1))
        {
            return BOI_AC_CONTINUE;
        }
    }

    return BOI_AC_STOP;
}


void ResizeAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_cref.Reset();
}


bool ResizeAction::AcceptKeyStream()
{
    return true;
}


bool ResizeAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand ResizeAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        m_xPrev = pEvent->x;
        m_yPrev = pEvent->y;

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Move) 
    {
        if (!m_cref.IsDestroyed())
        {
            qreal scaleFactor = pSI->ViewScaleFactor();

            DRef dref = pSI->NewData(BOI_STD_D(Point));
            QPointF* point = dref.GetWriteInstance<QPointF>();

            point->setX(0);
            point->setY(0);

            if ((m_axis == Axis_X) || (m_axis == Axis_XY))
            {
                qreal delta = pEvent->x - m_xPrev;
                delta /= scaleFactor;
                point->setX(delta);
            }

            if ((m_axis == Axis_Y) || (m_axis == Axis_XY))
            {
                qreal delta = pEvent->y - m_yPrev;
                delta /= scaleFactor;
                point->setY(delta);
            }

            pSI->EmitTo(m_cref, m_updateSizeReceiver, dref, true);

            m_xPrev = pEvent->x;
            m_yPrev = pEvent->y;
        }
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


ActionCommand ResizeAction::HandleKeyEvent(ASI* pSI, KeyEvent* pEvent)
{
    Q_UNUSED(pSI);

    if (pEvent->type == KeyEvent::Type_Press)
    {
        m_doSuspend = true;
        m_args.Set("InitialText", QChar(pEvent->key));

        return BOI_AC_SET(BOI_STD_A(TextInput), &m_args);
    }

    return BOI_AC_CONTINUE;
}


bool ResizeAction::Suspend(ASI* pSI)
{
    Q_UNUSED(pSI);

    return m_doSuspend;
}


ActionCommand ResizeAction::Resume(ASI* pSI)
{
    m_doSuspend = false;

    if (m_errorCode == 0)
    {
        if (!m_text.isEmpty())
        {
            qreal num1 = 0.0;
            qreal num2 = 0.0;

            bool num1Valid = false;
            bool num2Valid = false;

            QStringList parts = m_text.split(',');

            if (parts.size() > 0)
            {
                QString num1String = parts.at(0).trimmed();
                if (!num1String.isEmpty())
                {
                    num1 = num1String.toDouble(&num1Valid);
                }
            }

            if (parts.size() > 1)
            {
                QString num2String = parts.at(1).trimmed();
                if (!num2String.isEmpty())
                {
                    num2 = num2String.toDouble(&num2Valid);
                }
            }


            DRef dref = pSI->NewData(BOI_STD_D(Size));
            QSizeF* pSize = dref.GetWriteInstance<QSizeF>();

            pSize->setWidth(-1);
            pSize->setHeight(-1);

            if ((m_axis == Axis_X) && num1Valid)
            {
                pSize->setWidth(num1);
            }
            else if ((m_axis == Axis_Y) && num1Valid)
            {
                pSize->setHeight(num1);
            }
            else if (m_axis == Axis_XY)
            {
                if (num1Valid) pSize->setWidth(num1);
                if (num2Valid) pSize->setHeight(num2);
            }

            pSI->EmitTo(m_cref, m_setSizeReceiver, dref, true);
        }

        return BOI_AC_CONTINUE;
    }

    return BOI_AC_STOP;
}


} // namespace BOI

