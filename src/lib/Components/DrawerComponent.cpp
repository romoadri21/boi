/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPainter>
#include <QFontMetricsF>
#include <QStyleOptionGraphicsItem>
#include "Events/TouchEvent.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/DrawerComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(DrawerComponent)
    BOI_DECLARE_RECEIVER("{86316b23-125f-42b8-8ccc-151e8abca8ba}",
                         BOI_RECEIVER_FUNC(DrawerComponent, SetExtent))
BOI_END_RECEIVERS(DrawerComponent)


BOI_BEGIN_EMITTERS(DrawerComponent)
BOI_END_EMITTERS(DrawerComponent)


BOI_BEGIN_FUNCSETS(DrawerComponent)
BOI_END_FUNCSETS(DrawerComponent)


BOI_BEGIN_CALLERS(DrawerComponent)
BOI_END_CALLERS(DrawerComponent)


DrawerComponent::DrawerComponent()
    : Component(BOI_STD_C(Drawer))
{
}


bool DrawerComponent::Initialize()
{
    m_open = false;

    m_font.setPointSizeF(14);
    m_label = "Drawer";

    m_bgFill = QBrush(QColor(225, 225, 225));

    m_textPen = QPen(QColor(70, 70, 70));
    m_separatorPen = QPen(QColor(175, 175, 175));

    m_extent = QPointF(300, 500);

    UpdateFullRect();
    UpdateHandleRect();

    SetBoundingRect(m_handleRect);

    SetFlag(ComponentFlag_ClipsChildren);
    SetFlag(ComponentFlag_HandlesTouchEvents);

    return true;
}


void DrawerComponent::HandleTouchEvent(TouchEvent* pEvent)
{
    LockDraw();

    if (pEvent->type == TouchEvent::Type_Press)
    {
        if (m_open)
        {
            SetBoundingRect(m_handleRect);
        }
        else
        {
            SetBoundingRect(m_fullRect);
        }

        m_open = !m_open;
    }

    UnlockDraw();

    Update();
}


void DrawerComponent::Draw(QPainter* pPainter,
                           const QStyleOptionGraphicsItem* pOption)
{
    LockDraw();

    if (m_open)
    {
        QRectF exposedRect = m_fullRect & pOption->exposedRect;
        pPainter->fillRect(exposedRect, m_bgFill);

        pPainter->setPen(m_separatorPen);
        pPainter->drawLine(m_handleRect.topRight(), m_handleRect.bottomRight());
        pPainter->drawLine(m_handleRect.bottomLeft(), m_handleRect.bottomRight());
    }
    else
    {
        QRectF exposedRect = m_handleRect & pOption->exposedRect;
        pPainter->fillRect(exposedRect, m_bgFill);
    }

    pPainter->setFont(m_font);
    pPainter->setPen(m_textPen);
    pPainter->drawText(m_handleRect, Qt::AlignCenter, m_label);

    UnlockDraw();
}


void DrawerComponent::SetExtent(DRef& dref, int source)
{
    Q_UNUSED(source);

    bool doUpdate = false;

    if (dref.Type() == BOI_STD_D(Point))
    {
        LockDraw();

        QPointF newExtent = *dref.GetReadInstance<QPointF>();

        if (newExtent.x() > m_handleRect.width())
        {
            m_extent.setX(newExtent.x());
            doUpdate = true;
        }

        if (newExtent.y() > m_handleRect.height())
        {
            m_extent.setY(newExtent.y());
            doUpdate = true;
        }

        if (doUpdate)
        {
            UpdateFullRect();

            if (m_open)
            {
                Update();
            }
        }

        UnlockDraw();
    }
}


void DrawerComponent::UpdateFullRect()
{
    m_fullRect.setRect(0, 0, m_extent.x(), m_extent.y());
}


void DrawerComponent::UpdateHandleRect()
{
    QFontMetricsF fontMetrics(m_font);
    m_handleRect = fontMetrics.boundingRect(QRectF(0, 0, 1000, 1000),
                                            Qt::AlignCenter,
                                            m_label);

    qreal padding = fontMetrics.averageCharWidth();
    m_handleRect.adjust(-padding, -padding, padding, padding);
    m_handleRect.moveTo(QPointF(0, 0));
}


} // namespace BOI

