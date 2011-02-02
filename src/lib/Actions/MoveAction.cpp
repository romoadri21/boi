/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "Actions/MoveAction.h"


namespace BOI {


MoveAction::MoveAction()
    : Action(BOI_STD_A(Move))
{
    m_pMoveData = NULL;
}


ActionCommand MoveAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    pSI->SetCursor(Qt::OpenHandCursor);

    return BOI_AC_CONTINUE;
}


void MoveAction::Stop(ASI* pSI)
{
    ResetData();

    pSI->UnsetCursor();
}


bool MoveAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand MoveAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        pSI->SetCursor(Qt::ClosedHandCursor);

        QPoint point(pEvent->x, pEvent->y);

        int viewLayers = ViewLayerId_Main |
                         ViewLayerId_Overlay |
                         ViewLayerId_Underlay;

        CRefList crefs = pSI->ComponentsAtViewPoint(point, viewLayers);

        if (crefs.Count() > 0)
        {
            CRef cref = crefs.Value(0);

            if (pSI->IsSelected(cref))
            {
                crefs = pSI->Selection();
            }
            else
            {
                crefs = CRefList();
                crefs.Append(cref);
            }

            m_count = crefs.Count();
            Reserve(m_count);

            for (int i=0; i < m_count; i++)
            {
                cref = crefs.Value(i);

                m_pMoveData[i].cref = cref;

                QPointF origin = pSI->MapToParent(cref, QPointF(0, 0));
                QPointF touchPoint = pSI->MapFromViewToParent(cref, point);

                m_pMoveData[i].xDelta = touchPoint.x() - origin.x();
                m_pMoveData[i].yDelta = touchPoint.y() - origin.y();
            }
        }

        m_numTouchStreams++;
    }
    if (eventType == TouchEvent::Type_Move) 
    {
        for (int i=0; i < m_count; i++)
        {
            if (m_pMoveData[i].cref.IsValid())
            {
                QPointF point = pSI->MapFromViewToParent(m_pMoveData[i].cref,
                                                         QPoint(pEvent->x, pEvent->y));

                point -= QPointF(m_pMoveData[i].xDelta,
                                 m_pMoveData[i].yDelta);

                pSI->SetPosition(m_pMoveData[i].cref, point);
            }
        }
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        ResetData();

        pSI->SetCursor(Qt::OpenHandCursor);
        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


void MoveAction::Destroy()
{
    delete[] m_pMoveData;
}


void MoveAction::Reserve(int size)
{
    if (m_pMoveData == NULL)
    {
        m_pMoveData = new MoveData[size];
        m_capacity = size;
    }
    else
    {
        if (m_capacity < size)
        {
            delete[] m_pMoveData;

            m_pMoveData = new MoveData[size];
            m_capacity = size;
        }
    }
}


void MoveAction::ResetData()
{
    if (m_pMoveData != NULL)
    {
        for (int i=0; i < m_count; i++)
        {
            m_pMoveData[i].cref.Reset();
        }

        m_count = 0;
    }
}


} // namespace BOI

