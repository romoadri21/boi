/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QSizeF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "CSI.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/ImageComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(ImageComponent)
    BOI_DECLARE_RECEIVER("{7d4269b3-db0b-43cd-8afc-bd5e8251ac36}",
                         BOI_RECEIVER_FUNC(ImageComponent, SetImage))

    BOI_DECLARE_RECEIVER("{fb33af5a-942a-4e9d-814e-87d67228bc26}",
                         BOI_RECEIVER_FUNC(ImageComponent, SetSize))

    BOI_DECLARE_RECEIVER("{b06ff327-be49-4617-9391-f192b64b82f5}",
                         BOI_RECEIVER_FUNC(ImageComponent, UpdateSize))
BOI_END_RECEIVERS(ImageComponent)


BOI_BEGIN_EMITTERS(ImageComponent)
BOI_END_EMITTERS(ImageComponent)


BOI_BEGIN_FUNCSETS(ImageComponent)
BOI_END_FUNCSETS(ImageComponent)


BOI_BEGIN_CALLERS(ImageComponent)
BOI_END_CALLERS(ImageComponent)


ImageComponent::ImageComponent()
    : Component(BOI_STD_C(Image)),
      m_rect(),
      m_image()
{
}


bool ImageComponent::Initialize()
{
    m_rect.setRect(0, 0, 1, 1);
    SetBoundingRect(m_rect);

    return true;
}


void ImageComponent::Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption)
{
    Q_UNUSED(pOption);

    LockDraw();

    if (!m_image.isNull())
    {
        QRectF sourceRect = m_transform.mapRect(pOption->exposedRect);

        pPainter->drawImage(pOption->exposedRect, m_image, sourceRect);
    }

    UnlockDraw();
}


void ImageComponent::SetImage(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Image))
    {
        LockDraw();

        m_image = *dref.GetReadInstance<QImage>();

        m_rect.setWidth(m_image.width());
        m_rect.setHeight(m_image.height());

        UpdateTransform();
        SetBoundingRect(m_rect);

        UnlockDraw();

        Update();
    }
}


void ImageComponent::SetSize(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Size))
    {
        QSizeF newSize = *dref.GetReadInstance<QSizeF>();

        LockDraw();

        if (newSize.width() > 0) m_rect.setWidth(newSize.width());
        if (newSize.height() > 0) m_rect.setHeight(newSize.height());

        UpdateTransform();
        SetBoundingRect(m_rect);

        UnlockDraw();

        Update();
    }
}


void ImageComponent::UpdateSize(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Point))
    {
        QPointF point = *dref.GetReadInstance<QPointF>();

        LockDraw();

        m_rect.setWidth(m_rect.width() + point.x());
        m_rect.setHeight(m_rect.height() + point.y());

        UpdateTransform();
        SetBoundingRect(m_rect);

        UnlockDraw();

        Update();
    }
}


void ImageComponent::Import(const QHash<int, DRef>& in)
{
    DRef dref = in.value(1);
    SetImage(dref, -1);

    dref = in.value(2);
    SetSize(dref, -1);
}


void ImageComponent::Export(QHash<int, DRef>& out)
{
    DRef imageRef = SI()->NewData(BOI_STD_D(Image));
    DRef sizeRef = SI()->NewData(BOI_STD_D(Size));

    LockDraw();

    *imageRef.GetWriteInstance<QImage>() = m_image;
    *sizeRef.GetWriteInstance<QSizeF>() = m_rect.size();

    UnlockDraw();

    out.insert(1, imageRef);
    out.insert(2, sizeRef);
}


void ImageComponent::UpdateTransform()
{
    qreal xScale = m_rect.width() / m_image.width();
    qreal yScale = m_rect.height() / m_image.height();

    m_transform = QTransform::fromScale(xScale, yScale).inverted();
}


} // namespace BOI

