/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QList>
#include <QPointF>
#include "ASI.h"
#include "UuidType.h"
#include "CRefList.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/PasteAction.h"


#define BOI_CLIPBOARD_TAG "BOI.Clipboard"


namespace BOI {


PasteAction::PasteAction()
    : Action(BOI_STD_A(Paste))
{
}


ActionCommand PasteAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_uuid.clear();
    m_numTouchStreams = 0;

    QList<TypedUuid> uuids = pSI->GetTaggedUuids(BOI_CLIPBOARD_TAG,
                                                 UuidType_Collection);

    if (!uuids.isEmpty())
    {
        m_uuid = uuids.at(0).item1;

        pSI->SetCursor(Qt::CrossCursor);

        return BOI_AC_CONTINUE;
    }

    return BOI_AC_STOP;
}


void PasteAction::Stop(ASI* pSI)
{
    if (!m_uuid.isEmpty())
    {
        pSI->UnsetCursor();
    }
}


bool PasteAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand PasteAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        if (!m_uuid.isEmpty())
        {
            QPoint point(pEvent->x, pEvent->y);
            QPointF mappedPoint = pSI->MapFromViewToLayer(point);

            CRefList crefs = pSI->OpenCollection(m_uuid, mappedPoint);
            pSI->SetSelection(crefs);
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

