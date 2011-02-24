/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QSet>
#include <QList>
#include <QPointF>
#include <QStringList>
#include "ASI.h"
#include "UuidType.h"
#include "CRefList.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/InsertCollectionAction.h"


namespace BOI {


InsertCollectionAction::InsertCollectionAction()
    : Action(BOI_STD_A(InsertCollection))
{
    m_args.SetAutoDelete(false);
    m_args.SetPtr("TextPointer", &m_text);
    m_args.SetPtr("ErrorCode", &m_errorCode);
    m_args.Set("InitialText", "Enter keywords that have been associated with a collection...");
    m_args.Set("ClearOnNextPress", true);
}


ActionCommand InsertCollectionAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pArgs);

    m_uuid.clear();
    m_numTouchStreams = 0;

    return BOI_AC_SET(BOI_STD_A(TextInput), &m_args);
}


void InsertCollectionAction::Stop(ASI* pSI)
{
    if (!m_uuid.isEmpty())
    {
        pSI->UnsetCursor();
    }
}


bool InsertCollectionAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand InsertCollectionAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
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


bool InsertCollectionAction::Suspend(ASI* pSI)
{
    Q_UNUSED(pSI);

    return m_uuid.isEmpty();
}


ActionCommand InsertCollectionAction::Resume(ASI* pSI)
{
    if ((m_errorCode == 0) && !m_text.isEmpty())
    {
        QStringList parts = m_text.split(' ', QString::SkipEmptyParts);

        int numTags = parts.size();
        if (numTags > 0)
        {
            QSet<QString> set;

            for (int i=0; i < numTags; i++)
            {
                QList<TypedUuid> uuids = pSI->GetTaggedUuids(parts.at(i).trimmed(),
                                                             UuidType_Collection);

                int numUuids = uuids.size();
                for (int j=0; j < numUuids; j++)
                {
                    set.insert(uuids.at(j).item1);
                }
            }

            if (!set.isEmpty())
            {
                m_uuid = set.toList().at(0);

                pSI->SetCursor(Qt::CrossCursor);

                return BOI_AC_CONTINUE;
            }
        }
    }

    return BOI_AC_STOP;
}


} // namespace BOI

