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
#include "StandardComponents.h"
#include "Actions/TextInputAction.h"


namespace BOI {


TextInputAction::TextInputAction()
    : Action(BOI_STD_A(TextInput))
{
}


ActionCommand TextInputAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    m_componentActive = false;

    bool clearOnNext = false;
    QString initialText;

    m_pText = NULL;
    m_pErrorCode = NULL;

    if (pArgs != NULL)
    {
        if (pArgs->Contains("ErrorCode") &&
            pArgs->Contains("TextPointer"))
        {
            m_pErrorCode = pArgs->ValuePtr<int>("ErrorCode");
            m_pText = pArgs->ValuePtr<QString>("TextPointer");
        }

        if ((m_pText == NULL) ||
            (m_pErrorCode == NULL))
        {
            return BOI_AC_STOP;
        }

        *m_pErrorCode = 1;

        if (pArgs->Contains("ClearOnNextPress"))
        {
            clearOnNext = pArgs->Value<bool>("ClearOnNextPress");
        }

        if (pArgs->Contains("InitialText"))
        {
            initialText = pArgs->Value<QString>("InitialText");
        }
    }


    if (!m_inputComponent.IsValid())
    {
        m_inputComponent = pSI->NewComponent(BOI_STD_C(TextInput), ViewLayerId_Null);

        pSI->SetVisible(m_inputComponent, true);
        pSI->SetFlag(m_inputComponent, ComponentFlag_IsSelectable, false);

        m_funcSet = pSI->GetFuncSet(m_inputComponent, "{790e6f3f-4433-4490-a141-a4cb4433b0e7}");
        m_setTextReceiver = pSI->GetReceiver(m_inputComponent, "{7f2249e4-6c3c-40a5-9cff-59501f06ee37}");
        m_clearOnNextReceiver = pSI->GetReceiver(m_inputComponent, "{f58a0c2e-63c7-41e4-8895-6c7fa44ae32f}");

        if ((m_funcSet == -1) ||
            (m_setTextReceiver == -1) ||
            (m_clearOnNextReceiver == -1))
        {
            m_inputComponent.DestroyInstance();
            m_inputComponent.Reset();

            return BOI_AC_STOP;
        }
    }

    /*
     * Set the action type the
     * component should update.
     */

    DRef dref = pSI->NewData(BOI_STD_D(Int));
    *dref.GetWriteInstance<int>() = Type();

    pSI->CallFunc(m_inputComponent, m_funcSet, 0, dref);

    /*
     * Set clear on next press.
     */

    dref = pSI->NewData(BOI_STD_D(Bool));
    *dref.GetWriteInstance<bool>() = clearOnNext;

    pSI->EmitTo(m_inputComponent, m_clearOnNextReceiver, dref);

    /*
     * Set the text of the input component.
     */

    dref = pSI->NewData(BOI_STD_D(String));
    *dref.GetWriteInstance<QString>() = initialText;

    pSI->EmitTo(m_inputComponent, m_setTextReceiver, dref);

    /*
     * Set the key event focus to the input component.
     */
    m_prevKeyEventHandler = pSI->KeyEventHandler();
    pSI->SetKeyEventHandler(m_inputComponent);

    /*
     * Show the input component.
     */
    pSI->MoveToLayer(m_inputComponent, ViewLayerId_System);

    m_componentActive = true;

    return BOI_AC_CONTINUE;
}


void TextInputAction::Stop(ASI* pSI)
{
    if (m_componentActive)
    {
        pSI->MoveToLayer(m_inputComponent, ViewLayerId_Null);
        pSI->SetKeyEventHandler(m_prevKeyEventHandler);
    }

    m_prevKeyEventHandler.Reset();
}


void TextInputAction::Destroy()
{
    if (m_inputComponent.IsValid())
    {
        m_inputComponent.DestroyInstance();
    }

    m_inputComponent.Reset();
}


ActionCommand TextInputAction::Update(ASI* pSI, const ActionArgs* pArgs)
{
    pSI->MoveToLayer(m_inputComponent, ViewLayerId_Null);
    pSI->SetKeyEventHandler(m_prevKeyEventHandler);

    m_componentActive = false;

    if ((pArgs != NULL) && pArgs->Contains("Text"))
    {
        *m_pErrorCode = 0;
        *m_pText = pArgs->Value<QString>("Text");
    }

    return BOI_AC_STOP;
}


} // namespace BOI

