/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QStringList>
#include <QString>
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
    m_acceptKeyEvents = true;

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
        m_funcSet = pSI->GetFuncSet(m_cref, "{acb9a365-42b3-47b9-a2a7-a7c5f1999b0f}");

        if (m_funcSet != -1)
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

    m_cref.Reset();
}


void ResizeAction::Destroy()
{
    m_textInputComponent.Reset();
}


bool ResizeAction::AcceptKeyStream()
{
    return m_acceptKeyEvents;
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

            DRef dref;
            DRef sizeRef = pSI->CallFunc(m_cref, m_funcSet, 0, dref);

            QSizeF* pSize = sizeRef.GetWriteInstance<QSizeF>();

            if ((m_axis == Axis_X) || (m_axis == Axis_XY))
            {
                qreal delta = pEvent->x - m_xPrev;
                delta /= scaleFactor;
                pSize->rwidth() += delta;
            }

            if ((m_axis == Axis_Y) || (m_axis == Axis_XY))
            {
                qreal delta = pEvent->y - m_yPrev;
                delta /= scaleFactor;
                pSize->rheight() += delta;
            }

            pSI->CallFunc(m_cref, m_funcSet, 1, sizeRef);

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

        /*
         * Stop accepting key events so that
         * the events will be routed to the
         * text input component instead.
         */
        m_acceptKeyEvents = false;
    }

    return BOI_AC_CONTINUE;
}


ActionCommand ResizeAction::Update(ASI* pSI, const ActionArgs* pArgs)
{
    pSI->SetVisible(m_textInputComponent, false);
    pSI->SetKeyEventHandler(m_prevKeyEventHandler);

    m_acceptKeyEvents = true;

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


        DRef dref;
        DRef sizeRef = pSI->CallFunc(m_cref, m_funcSet, 0, dref);

        QSizeF* pSize = sizeRef.GetWriteInstance<QSizeF>();

        if ((m_axis == Axis_X) && num1Valid)
        {
            pSize->rwidth() = num1;
        }
        else if ((m_axis == Axis_Y) && num1Valid)
        {
            pSize->rheight() = num1;
        }
        else if (m_axis == Axis_XY)
        {
            if (num1Valid) pSize->rwidth() = num1;
            if (num2Valid) pSize->rheight() = num2;
        }

        pSI->CallFunc(m_cref, m_funcSet, 1, sizeRef);
    }

    return BOI_AC_CONTINUE;
}


} // namespace BOI

