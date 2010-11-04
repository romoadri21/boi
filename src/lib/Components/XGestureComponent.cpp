/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "StandardComponents.h"
#include "Components/XGestureComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(XGestureComponent)
BOI_END_RECEIVERS(XGestureComponent)


BOI_BEGIN_EMITTERS(XGestureComponent)
BOI_END_EMITTERS(XGestureComponent)


BOI_BEGIN_FUNCSETS(XGestureComponent)
BOI_END_FUNCSETS(XGestureComponent)


BOI_BEGIN_CALLERS(XGestureComponent)
BOI_END_CALLERS(XGestureComponent)


XGestureComponent::XGestureComponent()
    : Component(BOI_STD_C(XGesture)),
      m_boundingRect(0, 0, 100,100),
      m_image(100, 100, QImage::Format_ARGB32),
      m_radius(15.0)
{
}


bool XGestureComponent::Initialize()
{
    /* 
     * Initialize the image to have
     * a transparent background.
     */
    m_image.fill(0);

    QPainter painter(&m_image);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QColor(0, 0, 0));
    painter.drawLine(m_boundingRect.topLeft(), m_boundingRect.bottomRight());
    painter.drawLine(m_boundingRect.bottomLeft(), m_boundingRect.topRight());

    painter.setBrush(QColor(255, 255, 255));
    painter.drawEllipse(m_boundingRect.center(), m_radius, m_radius);

    SetOpacity(0.3);
    SetBoundingRect(m_boundingRect);
    return true;
}


void XGestureComponent::Draw(QPainter* pPainter,
                             const QStyleOptionGraphicsItem* pOption)
{
    pPainter->drawImage(pOption->exposedRect, m_image, pOption->exposedRect);
}


} // namespace BOI

