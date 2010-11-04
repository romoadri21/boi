/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/AppendTextAction.h"


namespace BOI {


AppendTextAction::AppendTextAction()
    : Action(BOI_STD_A(AppendText)),
      m_target()
{
}


ActionCommand AppendTextAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    m_target = pSI->ActiveComponent();

    if (m_target.IsValid())
    {
        m_appendTextReceiver = pSI->GetReceiver(m_target,
                                                "{b01ea5e1-a4b9-4114-873f-376ab652a554}");

        if (m_appendTextReceiver != -1)
        {
            return BOI_AC_CONTINUE;
        }
        else
        {
            m_target.Reset();
        }
    }

    return BOI_AC_STOP;
}


void AppendTextAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_target.Reset();
}


bool AppendTextAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand AppendTextAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        QPoint point(pEvent->x, pEvent->y);

        int sceneLayers = SceneLayerId_Main |
                          SceneLayerId_Overlay |
                          SceneLayerId_Underlay;

        CRefList crefs = pSI->ComponentsAtViewPoint(point, sceneLayers);

        if (crefs.Count() > 0)
        {
            CRef cref = crefs.Value(0);
            int getTextFuncSet = pSI->GetFuncSet(cref,
                                                 "{c768ae35-9c07-46d8-be25-3ad3f65ccdfd}");

            if (getTextFuncSet != -1)
            {
                DRef input;
                DRef output;
                output = pSI->CallFunc(cref, getTextFuncSet, 0, input);
                pSI->EmitTo(m_target, m_appendTextReceiver, output, true);
            }
        }

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Release)
    {
        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


} // namespace BOI

