/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QString>
#include "ASI.h"
#include "CRef.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/InsertAction.h"


namespace BOI {


InsertAction::InsertAction()
    : Action(BOI_STD_A(Insert))
{
}


ActionCommand InsertAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    m_numTouchStreams = 0;

    if (pArgs->Contains("UUID"))
    {
        QString uuid = pArgs->Value<QString>("UUID");
        m_typeId = pSI->ConvertUuid_C(uuid);

        if (m_typeId != -1)
        {
            pSI->SetCursor(Qt::CrossCursor);
            return BOI_AC_CONTINUE;
        }
    }

    return BOI_AC_STOP;
}


void InsertAction::Stop(ASI* pSI)
{
    pSI->UnsetCursor();
}


bool InsertAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand InsertAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        CRef cref = pSI->NewComponent(m_typeId);

        QPointF point = pSI->MapFromViewToLayer(QPoint(pEvent->x, pEvent->y));
        pSI->CenterComponentOn(cref, point);
        pSI->SetVisible(cref, true);

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


} // namespace BOI

