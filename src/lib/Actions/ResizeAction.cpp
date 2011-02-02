/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QStringList>
#include <QString>
#include <QPointF>
#include <QSizeF>
#include "ASI.h"
#include "ActionArgs.h"
#include "ViewLayerId.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Events/KeyEvent.h"
#include "Events/TouchEvent.h"
#include "Actions/ResizeAction.h"


namespace BOI {


ResizeAction::ResizeAction()
    : Action(BOI_STD_A(Resize))
{
}


ActionCommand ResizeAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    m_numTouchStreams = 0;
    m_textInputActive = false;

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
        else
        {
            m_cref.Reset();
        }
    }

    return BOI_AC_STOP;
}


void ResizeAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    if (m_textInputActive)
    {
        pSI->SetVisible(m_textInputComponent, false);
        pSI->SetKeyEventHandler(m_prevKeyEventHandler);

        m_prevKeyEventHandler.Reset();
    }

    m_cref.Reset();
}


void ResizeAction::Destroy()
{
    m_textInputComponent.Reset();
}


bool ResizeAction::AcceptKeyStream()
{
    /*
     * Stop accepting key events when the
     * TextInput component is active so
     * that the events will be routed to
     * the text input component instead
     * of to 'this' action.
     */
    return !m_textInputActive;
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
    if (pEvent->type == KeyEvent::Type_Press)
    {
        if (!m_textInputComponent.IsValid())
        {
            m_textInputComponent = pSI->NewComponent(BOI_STD_C(TextInput),
                                                     ViewLayerId_System);

            int funcSet = pSI->GetFuncSet(m_textInputComponent,
                                          "{790e6f3f-4433-4490-a141-a4cb4433b0e7}");
            if (funcSet != -1)
            {
                /*
                 * Set the Action type id so that
                 * Update gets called correctly.
                 */

                DRef dref = pSI->NewData(BOI_STD_D(Int));
                *dref.GetWriteInstance<int>() = Type();
                pSI->CallFunc(m_textInputComponent, funcSet, 0, dref);
            }

            int receiver = pSI->GetReceiver(m_textInputComponent,
                                            "{f58a0c2e-63c7-41e4-8895-6c7fa44ae32f}");
            if (receiver != -1)
            {
                /*
                 * Tell the component to keep the
                 * text after a new key press.
                 */

                DRef dref = pSI->NewData(BOI_STD_D(Bool));
                *dref.GetWriteInstance<bool>() = false;

                pSI->EmitTo(m_textInputComponent, receiver, dref);
            }

            m_setTextReceiver = pSI->GetReceiver(m_textInputComponent,
                                                 "{7f2249e4-6c3c-40a5-9cff-59501f06ee37}");
        }

        if (m_setTextReceiver != -1)
        {
            /*
             * Set the text to the key pressed.
             */

            DRef dref = pSI->NewData(BOI_STD_D(String));
            *dref.GetWriteInstance<QString>() = QChar(pEvent->key);

            pSI->EmitTo(m_textInputComponent, m_setTextReceiver, dref);
        }

        m_prevKeyEventHandler = pSI->KeyEventHandler();

        pSI->SetKeyEventHandler(m_textInputComponent);
        pSI->SetVisible(m_textInputComponent, true);

        m_textInputActive = true;
    }

    return BOI_AC_CONTINUE;
}


ActionCommand ResizeAction::Update(ASI* pSI, const ActionArgs* pArgs)
{
    pSI->SetVisible(m_textInputComponent, false);
    pSI->SetKeyEventHandler(m_prevKeyEventHandler);

    m_textInputActive = false;

    if ((pArgs != NULL) &&
        (pArgs->Contains("Text")))
    {
        qreal num1 = 0.0;
        qreal num2 = 0.0;

        bool num1Valid = false;
        bool num2Valid = false;

        QString text = pArgs->Value<QString>("Text");
        QStringList parts = text.split(',');

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


} // namespace BOI

