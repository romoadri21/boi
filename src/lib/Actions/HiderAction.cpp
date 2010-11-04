/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "Actions/HiderAction.h"


namespace BOI {


HiderAction::HiderAction()
    : Action(BOI_STD_A(Hider)),
      m_cref()
{
}


ActionCommand HiderAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pSI);

    if (pArgs->Contains("CRef"))
    {
        m_cref = pArgs->Value<CRef>("CRef");

        if (m_cref.IsValid())
        {
            return BOI_AC_CONTINUE;
        }
    }

    return BOI_AC_STOP;
}


void HiderAction::Stop(ASI* pSI)
{
    if (m_cref.IsValid())
    {
        if (!m_cref.IsDestroyed())
        {
            pSI->SetVisible(m_cref, false);
        }

        m_cref.Reset();
    }
}


} // namespace BOI

