/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "Actions/SetTextAction.h"


namespace BOI {


SetTextAction::SetTextAction()
    : Action(BOI_STD_A(SetText))
{
}


ActionCommand SetTextAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_activeComponent = pSI->ActiveComponent();

    if (m_activeComponent.IsValid())
    {
        if (!m_inputComponent.IsValid() &&
            !m_inputComponent.Initialize(pSI))
        {
            return BOI_AC_STOP;
        }

        m_prevKeyEventHandler = pSI->KeyEventHandler();

        m_inputComponent.SetAction(pSI, Type());
        m_inputComponent.SetClearOnNextPress(pSI, true);
        m_inputComponent.SetText(pSI, "Enter the text to load into the component...");
        m_inputComponent.SetFocus(pSI);
        m_inputComponent.Show(pSI);

        return BOI_AC_CONTINUE;
    }

    return BOI_AC_STOP;
}


void SetTextAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    if (m_inputComponent.IsVisible())
    {
        m_inputComponent.Hide(pSI);

        pSI->SetKeyEventHandler(m_prevKeyEventHandler);
    }

    m_activeComponent.Reset();
    m_prevKeyEventHandler.Reset();
}


void SetTextAction::Destroy()
{
    m_inputComponent.Destroy();
}


ActionCommand SetTextAction::Update(ASI* pSI, const ActionArgs* pArgs)
{
    m_inputComponent.Hide(pSI);

    pSI->SetKeyEventHandler(m_prevKeyEventHandler);

    if ((pArgs != NULL) &&
        (pArgs->Contains("Text")))
    {
        QString text = pArgs->Value<QString>("Text");

        int receiver = pSI->GetReceiver(m_activeComponent,
                                        "{7f2249e4-6c3c-40a5-9cff-59501f06ee37}");
        if (receiver != -1)
        {
            DRef dref = pSI->NewData(BOI_STD_D(String));
            *dref.GetWriteInstance<QString>() = text;

            pSI->EmitTo(m_activeComponent, receiver, dref, true);
        }
    }

    return BOI_AC_STOP;
}


} // namespace BOI

