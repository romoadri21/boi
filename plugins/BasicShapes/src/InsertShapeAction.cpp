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
#include "InsertShapeAction.h"


InsertShapeAction::InsertShapeAction(int type)
    : Action(type)
{
}


BOI::ActionCommand InsertShapeAction::Start(BOI::ASI* pSI, const BOI::ActionArgs* pArgs)
{
    m_numTouchStreams = 0;

    if ((pArgs != NULL) && pArgs->Contains("UUID"))
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


void InsertShapeAction::Stop(BOI::ASI* pSI)
{
    pSI->UnsetCursor();
}


bool InsertShapeAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


BOI::ActionCommand InsertShapeAction::HandleTouchEvent(BOI::ASI* pSI, BOI::TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == BOI::TouchEvent::Type_Press)
    {
        BOI::CRef cref = pSI->NewComponent(m_typeId);

        QPointF point = pSI->MapFromViewToScene(QPoint(pEvent->x, pEvent->y));
        pSI->SetPosition(cref, point);

        pSI->SetActiveComponent(cref);
        pSI->SetVisible(cref, true);

        m_numTouchStreams++;
    }
    else if (eventType == BOI::TouchEvent::Type_Release) 
    {
        m_numTouchStreams--;

        return BOI_AC_REPLACE(BOI_STD_A(Resize), NULL);
    }

    return BOI_AC_CONTINUE;
}

