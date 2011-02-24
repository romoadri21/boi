/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "Actions/SetTextAction.h"


namespace BOI {


SetTextAction::SetTextAction()
    : Action(BOI_STD_A(SetText))
{
    m_args.SetAutoDelete(false);
    m_args.SetPtr("TextPointer", &m_text);
    m_args.SetPtr("ErrorCode", &m_errorCode);
    m_args.Set("InitialText", "Enter the text to load into the component...");
    m_args.Set("ClearOnNextPress", true);
}


ActionCommand SetTextAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_activeComponent = pSI->ActiveComponent();

    if (m_activeComponent.IsValid())
    {
        return BOI_AC_SET(BOI_STD_A(TextInput), &m_args);
    }

    return BOI_AC_STOP;
}


void SetTextAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_activeComponent.Reset();
}


bool SetTextAction::Suspend(ASI* pSI)
{
    Q_UNUSED(pSI);

    return true;
}


ActionCommand SetTextAction::Resume(ASI* pSI)
{
    if ((m_errorCode == 0) && !m_text.isEmpty())
    {
        int receiver = pSI->GetReceiver(m_activeComponent,
                                        "{7f2249e4-6c3c-40a5-9cff-59501f06ee37}");
        if (receiver != -1)
        {
            DRef dref = pSI->NewData(BOI_STD_D(String));
            *dref.GetWriteInstance<QString>() = m_text;

            pSI->EmitTo(m_activeComponent, receiver, dref, true);
        }
    }

    return BOI_AC_STOP;
}


} // namespace BOI

