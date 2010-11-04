/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/RemoveFromLayoutAction.h"


namespace BOI {


RemoveFromLayoutAction::RemoveFromLayoutAction()
    : Action(BOI_STD_A(RemoveFromLayout)),
      m_layout()
{
}


ActionCommand RemoveFromLayoutAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    m_layout = pSI->ActiveComponent();

    if (m_layout.IsValid())
    {
        if (pSI->IsLayout(m_layout))
        {
            return BOI_AC_CONTINUE;
        }
        else
        {
            m_layout.Reset();
        }
    }

    return BOI_AC_STOP;
}


void RemoveFromLayoutAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_layout.Reset();
}


bool RemoveFromLayoutAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand RemoveFromLayoutAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
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
            pSI->RemoveFromLayout(cref, m_layout);
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

