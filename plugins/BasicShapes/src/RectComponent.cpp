/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QSizeF>
#include <QVariant>
#include <QPainter>
#include "CSI.h"
#include "StateId.h"
#include "StandardDataTypes.h"
#include "RectComponent.h"


BOI_BEGIN_RECEIVERS(RectComponent)
    BOI_DECLARE_RECEIVER("{fb33af5a-942a-4e9d-814e-87d67228bc26}",
                         BOI_RECEIVER_FUNC(RectComponent, SetSize))

    BOI_DECLARE_RECEIVER("{b06ff327-be49-4617-9391-f192b64b82f5}",
                         BOI_RECEIVER_FUNC(RectComponent, UpdateSize))

    BOI_DECLARE_RECEIVER("{481d22f7-cdff-41f9-b2de-163754395dab}",
                         BOI_RECEIVER_FUNC(RectComponent, SetPen))

    BOI_DECLARE_RECEIVER("{690e2ab4-e24c-4233-a7d3-b8533e2d3e93}",
                         BOI_RECEIVER_FUNC(RectComponent, SetBrush))

    BOI_DECLARE_RECEIVER("{7a2a089c-8ca9-4813-bba3-14aa55cae552}",
                         BOI_RECEIVER_FUNC(RectComponent, SetRadii))

    BOI_DECLARE_RECEIVER("{041ca99e-2564-4ddc-84c9-4538e8f75379}",
                         BOI_RECEIVER_FUNC(RectComponent, SetBorderWidth))
BOI_END_RECEIVERS(RectComponent)


BOI_BEGIN_EMITTERS(RectComponent)
BOI_END_EMITTERS(RectComponent)


BOI_BEGIN_FUNCSETS(RectComponent)
BOI_END_FUNCSETS(RectComponent)


BOI_BEGIN_CALLERS(RectComponent)
BOI_END_CALLERS(RectComponent)


RectComponent::RectComponent(int type)
    : Component(type),
      m_rect(),
      m_boundingRect(),
      m_xRadius(0.0),
      m_yRadius(0.0),
      m_pen(),
      m_brush()
{
}


bool RectComponent::Initialize()
{
    m_xRadius = 0.0;
    m_yRadius = 0.0;

    m_rect.setRect(0, 0, 100, 100);

    QVariant var;
    SI()->GetState(BOI::StateId_Pen, var);
    m_pen = var.value<QPen>();

    m_pen.setJoinStyle(Qt::MiterJoin);

    SI()->GetState(BOI::StateId_Brush, var);
    m_brush = var.value<QBrush>();

    UpdateRect();

    return true;
}


void RectComponent::Draw(QPainter* pPainter,
                         const QStyleOptionGraphicsItem* pOption)
{
    Q_UNUSED(pOption);

    LockDraw();

    pPainter->setPen(m_pen);
    pPainter->setBrush(m_brush);
    pPainter->setRenderHint(QPainter::Antialiasing, true);
    pPainter->drawRoundedRect(m_rect, m_xRadius, m_yRadius);
    pPainter->setRenderHint(QPainter::Antialiasing, false);

    UnlockDraw();
}


void RectComponent::SetSize(BOI::DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Size))
    {
        QSizeF newSize = *dref.GetReadInstance<QSizeF>();

        LockDraw();

        if (newSize.width() > 0) m_rect.setWidth(newSize.width());
        if (newSize.height() > 0) m_rect.setHeight(newSize.height());

        UpdateRect();

        UnlockDraw();
    }
}


void RectComponent::UpdateSize(BOI::DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Point))
    {
        QPointF point = *dref.GetReadInstance<QPointF>();

        LockDraw();

        m_rect.setWidth(m_rect.width() + point.x());
        m_rect.setHeight(m_rect.height() + point.y());

        UpdateRect();

        UnlockDraw();
    }
}


void RectComponent::SetPen(BOI::DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Pen))
    {
        LockDraw();

        m_pen = *dref.GetReadInstance<QPen>();
        UpdateRect();

        UnlockDraw();
    }
}


void RectComponent::SetBrush(BOI::DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Brush))
    {
        LockDraw();

        m_brush = *dref.GetReadInstance<QBrush>();

        UnlockDraw();

        Update();
    }
}


void RectComponent::SetRadii(BOI::DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Size))
    {
        QSizeF newRadii = *dref.GetReadInstance<QSizeF>();

        LockDraw();

        m_xRadius = (newRadii.width() > 0.0) ? newRadii.width() : 0.0;
        m_yRadius = (newRadii.height() > 0.0) ? newRadii.height() : 0.0;

        UnlockDraw();

        Update();
    }
}


void RectComponent::SetBorderWidth(BOI::DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Double))
    {
        LockDraw();

        m_pen.setWidthF(*dref.GetReadInstance<double>());
        UpdateRect();

        UnlockDraw();
    }
}


void RectComponent::UpdateRect()
{
    qreal penWidth = 0.0;

    if (m_pen.style() != Qt::NoPen)
    {
        penWidth = (m_pen.isCosmetic()) ? 1 : m_pen.widthF();
    }

    qreal halfPenWidth = penWidth / 2;

    m_rect.moveTo(halfPenWidth, halfPenWidth);
    m_boundingRect.setRect(0, 0,
                           m_rect.width()  + penWidth,
                           m_rect.height() + penWidth);

    SetBoundingRect(m_boundingRect);
    Update();
}


void RectComponent::Import(const QHash<int, BOI::DRef>& in)
{
    BOI::DRef dref = in.value(1);

    if (dref.Type() == BOI_STD_D(Size))
    {
        QSizeF newSize = *dref.GetReadInstance<QSizeF>();

        LockDraw();

        m_rect.setSize(newSize);
        UpdateRect();

        UnlockDraw();
    }
}


void RectComponent::Export(QHash<int, BOI::DRef>& out)
{
    BOI::DRef sizeRef = SI()->NewData(BOI_STD_D(Size));

    LockDraw();

    *sizeRef.GetWriteInstance<QSizeF>() = m_rect.size();

    UnlockDraw();

    out.insert(1, sizeRef);
}

