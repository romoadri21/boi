/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QUrl>
#include "ASI.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "Actions/LoadUrlAction.h"


namespace BOI {


LoadUrlAction::LoadUrlAction()
    : Action(BOI_STD_A(LoadUrl))
{
}


ActionCommand LoadUrlAction::Start(ASI* pSI, const ActionArgs* pArgs)
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
        m_inputComponent.SetText(pSI, "Enter the URL to load into the browser component...");
        m_inputComponent.SetFocus(pSI);
        m_inputComponent.Show(pSI);

        return BOI_AC_CONTINUE;
    }

    return BOI_AC_STOP;
}


void LoadUrlAction::Stop(ASI* pSI)
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


void LoadUrlAction::Destroy()
{
    m_inputComponent.Destroy();
}


ActionCommand LoadUrlAction::Update(ASI* pSI, const ActionArgs* pArgs)
{
    m_inputComponent.Hide(pSI);

    pSI->SetKeyEventHandler(m_prevKeyEventHandler);

    if ((pArgs != NULL) &&
        (pArgs->Contains("Text")))
    {
        QString text = pArgs->Value<QString>("Text");

        QUrl url = QUrl::fromUserInput(text);

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

