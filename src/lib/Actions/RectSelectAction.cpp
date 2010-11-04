/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QVariant>
#include <QPointF>
#include <QSizeF>
#include <QBrush>
#include <QPen>
#include "ASI.h"
#include "StateId.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "Events/TouchEvent.h"
#include "Actions/RectSelectAction.h"


namespace BOI {


RectSelectAction::RectSelectAction()
    : Action(BOI_STD_A(RectSelect))
{
}


ActionCommand RectSelectAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_numTouchStreams = 0;

    if (!m_rectComponent.IsValid())
    {
        int typeId = pSI->ConvertUuid_C("{5899283b-cfd6-4dfc-9544-4a4608d27fd9}");

        if (typeId != -1)
        {
            m_rectComponent = pSI->NewComponent(typeId);

            pSI->SetSelectable(m_rectComponent, false);

            m_setSizeReceiver = pSI->GetReceiver(m_rectComponent,
                                                 "{fb33af5a-942a-4e9d-814e-87d67228bc26}");

            /*
             * Set the border of the RectComponent to
             * a black dashed line.
             */

            DRef dref = pSI->NewData(BOI_STD_D(Pen));
            *dref.GetWriteInstance<QPen>() = QPen(Qt::DashLine);

            int receiver = pSI->GetReceiver(m_rectComponent, "{481d22f7-cdff-41f9-b2de-163754395dab}");
            pSI->EmitTo(m_rectComponent, receiver, dref);

            /*
             * Set the fill of the RectComponent to no fill.
             */

            dref = pSI->NewData(BOI_STD_D(Brush));
            *dref.GetWriteInstance<QBrush>() = QBrush(Qt::NoBrush);

            receiver = pSI->GetReceiver(m_rectComponent, "{690e2ab4-e24c-4233-a7d3-b8533e2d3e93}");
            pSI->EmitTo(m_rectComponent, receiver, dref);
        }
        else
        {
            return BOI_AC_STOP;
        }
    }

    pSI->StackOnTop(m_rectComponent);

    return BOI_AC_CONTINUE;
}


void RectSelectAction::Destroy()
{
    m_rectComponent.Reset();
}


bool RectSelectAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand RectSelectAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        QVariant var;

        pSI->GetState(StateId_InnerViewRect, var);
        m_innerViewRect = var.toRectF();

        pSI->GetState(StateId_ScrollVelocity, var);
        m_scrollVelocity = var.toDouble();

        m_startX = pEvent->x;
        m_startY = pEvent->y;

        m_scrollerActive = false;

        m_scaleFactor = pSI->ViewScaleFactor();

        QPointF startPoint = pSI->MapFromViewToScene(QPoint(pEvent->x, pEvent->y));
        m_selectionRect.SetStartPoint(startPoint);

        QPointF endPoint = pSI->MapFromViewToScene(QPoint(pEvent->x + 1, pEvent->y + 1));
        m_selectionRect.SetEndPoint(endPoint);

        UpdateRectComponent(pSI);

        pSI->SetVisible(m_rectComponent, true);

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Move)
    {
        m_x = pEvent->x;
        m_y = pEvent->y;

        if (!m_scrollerActive)
        {
            if (m_innerViewRect.contains(m_x, m_y))
            {
                QPointF endPoint = pSI->MapFromViewToScene(QPoint(pEvent->x, pEvent->y));
                m_selectionRect.SetEndPoint(endPoint);

                UpdateRectComponent(pSI);

                m_startX = m_x;
                m_startY = m_y;
            }
            else
            {
                qreal args[3];
                args[0] = m_startX - m_x;
                args[1] = m_startY - m_y;
                args[2] = m_scrollVelocity / m_scaleFactor;

                m_scrollerId = pSI->AppendViewTransformer(BOI_VTID(Scroll), 1, args);

                if (m_scrollerId != -1)
                {
                    m_scrollerActive = true;
                }
            }
        }
        else
        {
            if (m_innerViewRect.contains(m_x, m_y))
            {
                m_scrollerActive = false;
                pSI->StopViewTransformer(m_scrollerId);

                QPointF endPoint = pSI->MapFromViewToScene(QPoint(pEvent->x, pEvent->y));
                m_selectionRect.SetEndPoint(endPoint);

                UpdateRectComponent(pSI);

                m_startX = m_x;
                m_startY = m_y;
            }
            else
            {
                qreal args[3];
                args[0] = m_startX - m_x;
                args[1] = m_startY - m_y;
                args[2] = m_scrollVelocity / m_scaleFactor;

                if (!pSI->UpdateViewTransformer(m_scrollerId, args))
                {
                    m_scrollerActive = false;
                }
            }
        }
    }
    else if (eventType == TouchEvent::Type_Release)
    {
        if (m_scrollerActive)
        {
            m_scrollerActive = false;
            pSI->StopViewTransformer(m_scrollerId);
        }

        pSI->SetVisible(m_rectComponent, false);

        CRefList crefs = pSI->ComponentsInLayerRect(m_selectionRect.Rect(),
                                                    SceneLayerId_Main);
        pSI->SetSelection(crefs);

        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


void RectSelectAction::HandleViewTransformed(ASI* pSI)
{
    QPointF endPoint = pSI->MapFromViewToScene(QPoint(m_x, m_y));
    m_selectionRect.SetEndPoint(endPoint);

    UpdateRectComponent(pSI);
}


void RectSelectAction::UpdateRectComponent(ASI* pSI)
{
    DRef dref = pSI->NewData(BOI_STD_D(Size));
    *dref.GetWriteInstance<QSizeF>() = m_selectionRect.Rect().size();

    pSI->EmitTo(m_rectComponent, m_setSizeReceiver, dref);

    pSI->SetPosition(m_rectComponent, m_selectionRect.Rect().topLeft());
}


} // namespace BOI

