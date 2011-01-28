/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "StandardComponents.h"
#include "Events/TouchEvent.h"
#include "Actions/InsertBrowserAction.h"


namespace BOI {


InsertBrowserAction::InsertBrowserAction()
    : Action(BOI_STD_A(InsertBrowser))
{
}


ActionCommand InsertBrowserAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    pSI->SetCursor(Qt::CrossCursor);

    return BOI_AC_CONTINUE;
}


void InsertBrowserAction::Stop(ASI* pSI)
{
    pSI->UnsetCursor();
}


bool InsertBrowserAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand InsertBrowserAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        CRef cref = pSI->NewComponent(BOI_STD_C(Browser));

        QPointF point = pSI->MapFromViewToLayer(QPoint(pEvent->x, pEvent->y));
        pSI->CenterComponentOn(cref, point);

        pSI->SetKeyEventHandler(cref);
        pSI->SetVisible(cref, true);

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        m_numTouchStreams--;
        return BOI_AC_STOP;
    }

    return BOI_AC_CONTINUE;
}


} // namespace BOI

