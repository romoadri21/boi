/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/SetChildAction.h"


namespace BOI {


SetChildAction::SetChildAction()
    : Action(BOI_STD_A(SetChild)),
      m_cref()
{
}


ActionCommand SetChildAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);
    m_numTouchStreams = 0;

    m_cref = pSI->ActiveComponent();

    if (!m_cref.IsValid())
    {
        return BOI_AC_STOP;
    }

    return BOI_AC_CONTINUE;
}


void SetChildAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_cref.Reset();
}


bool SetChildAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand SetChildAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        QPoint point(pEvent->x, pEvent->y);

        int viewLayers = ViewLayerId_Main |
                         ViewLayerId_Overlay |
                         ViewLayerId_Underlay;

        CRefList crefs = pSI->ComponentsAtViewPoint(point, viewLayers);

        if (crefs.Count() > 0)
        {
            CRef child = crefs.Value(0);

            if (pSI->IsSelected(child))
            {
                crefs = pSI->Selection();
                int numCRefs = crefs.Count();

                for (int i=0; i < numCRefs; i++)
                {
                    child = crefs.Value(i);
                    pSI->SetParent(child, m_cref);
                }
            }
            else
            {
                pSI->SetParent(child, m_cref);
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

