/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Utilities/TextInputWrapper.h"


namespace BOI {


TextInputWrapper::TextInputWrapper()
    : m_funcSet(-1),
      m_setTextReceiver(-1),
      m_clearOnNextReceiver(-1)
{
}


bool TextInputWrapper::Initialize(ASI* pSI)
{
    m_cref = pSI->NewComponent(BOI_STD_C(TextInput), ViewLayerId_Null);

    m_visible = false;

    pSI->SetVisible(m_cref, true);
    pSI->SetFlag(m_cref, ComponentFlag_IsSelectable, false);

    m_funcSet = pSI->GetFuncSet(m_cref, "{790e6f3f-4433-4490-a141-a4cb4433b0e7}");
    m_setTextReceiver = pSI->GetReceiver(m_cref, "{7f2249e4-6c3c-40a5-9cff-59501f06ee37}");
    m_clearOnNextReceiver = pSI->GetReceiver(m_cref, "{f58a0c2e-63c7-41e4-8895-6c7fa44ae32f}");

    if ((m_funcSet == -1) ||
        (m_setTextReceiver == -1) ||
        (m_clearOnNextReceiver == -1))
    {
        m_cref.DestroyInstance();
        m_cref.Reset();

        return false;
    }

    return true;
}


void TextInputWrapper::Destroy()
{
    if (m_cref.IsValid())
    {
        m_cref.DestroyInstance();
    }

    m_cref.Reset();
}


void TextInputWrapper::SetAction(ASI* pSI, int actionId)
{
    DRef dref = pSI->NewData(BOI_STD_D(Int));
    *dref.GetWriteInstance<int>() = actionId;

    pSI->CallFunc(m_cref, m_funcSet, 0, dref);
}


void TextInputWrapper::SetText(ASI* pSI, const QString& text)
{
    DRef dref = pSI->NewData(BOI_STD_D(String));
    *dref.GetWriteInstance<QString>() = text;

    pSI->EmitTo(m_cref, m_setTextReceiver, dref);
}


void TextInputWrapper::SetClearOnNextPress(ASI* pSI, bool clear)
{
    DRef dref = pSI->NewData(BOI_STD_D(Bool));
    *dref.GetWriteInstance<bool>() = clear;

    pSI->EmitTo(m_cref, m_clearOnNextReceiver, dref);
}


void TextInputWrapper::Show(ASI* pSI)
{
    pSI->MoveToLayer(m_cref, ViewLayerId_System);
    m_visible = true;
}


void TextInputWrapper::Hide(ASI* pSI)
{
    pSI->MoveToLayer(m_cref, ViewLayerId_Null);
    m_visible = false;
}


bool TextInputWrapper::IsValid()
{
    if ((m_cref.IsValid()) &&
        (m_funcSet != -1) &&
        (m_setTextReceiver != -1) &&
        (m_clearOnNextReceiver != -1)) return true;

    return false;
}


bool TextInputWrapper::IsVisible()
{
    return m_visible;
}


void TextInputWrapper::SetFocus(ASI* pSI)
{
    pSI->SetKeyEventHandler(m_cref);
}


} // namespace BOI

