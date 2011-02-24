/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QUrl>
#include "ASI.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "Actions/LoadUrlAction.h"


namespace BOI {


LoadUrlAction::LoadUrlAction()
    : Action(BOI_STD_A(LoadUrl))
{
    m_args.SetAutoDelete(false);
    m_args.SetPtr("TextPointer", &m_text);
    m_args.SetPtr("ErrorCode", &m_errorCode);
    m_args.Set("InitialText", "Enter the URL to load into the browser component...");
    m_args.Set("ClearOnNextPress", true);
}


ActionCommand LoadUrlAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_activeComponent = pSI->ActiveComponent();

    if (m_activeComponent.IsValid())
    {
        return BOI_AC_SET(BOI_STD_A(TextInput), &m_args);
    }

    return BOI_AC_STOP;
}


void LoadUrlAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_activeComponent.Reset();
}


bool LoadUrlAction::Suspend(ASI* pSI)
{
    Q_UNUSED(pSI);

    return true;
}


ActionCommand LoadUrlAction::Resume(ASI* pSI)
{
    if ((m_errorCode == 0) && !m_text.isEmpty())
    {
        QUrl url = QUrl::fromUserInput(m_text);

        if (url.isValid())
        {
            int receiver = pSI->GetReceiver(m_activeComponent,
                                            "{7473aebc-56e3-4dc4-994e-da426f45866c}");
            if (receiver != -1)
            {
                DRef dref = pSI->NewData(BOI_STD_D(String));
                *dref.GetWriteInstance<QString>() = url.toString();

                pSI->EmitTo(m_activeComponent, receiver, dref);
            }
        }
    }

    return BOI_AC_STOP;
}


} // namespace BOI

