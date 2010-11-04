/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "Component.h"
#include "ComponentData.h"
#include "GraphicsItem.h"


namespace BOI {


GraphicsItem::GraphicsItem(Component* pComponent)
    : QGraphicsItem(),
      m_pComponent(pComponent),
      m_boundingRect(0, 0, 1, 1)
{
}


void GraphicsItem::SetBoundingRect(const QRectF& rect)
{
    prepareGeometryChange();
    m_boundingRect = rect;
}


QRectF GraphicsItem::boundingRect() const
{
    return m_boundingRect;
}


void GraphicsItem::paint(QPainter *pPainter,
                         const QStyleOptionGraphicsItem *pOption,
                         QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    /*
     * Set the painter to clip drawing outside of the exposed rect.
     * This will allow components to freely draw within paint(...)
     * without having to worry about staying within the exposed
     * rect which could potentially complicate a components drawing
     * code.
     * Note, the bounding rect can not be used here because it could
     * potentially allow drawing outside of the update region (if
     * exposedRect is smaller than boundingRect()) and thus cause
     * artifacts since only the exposed region should be updated.
     */
    pPainter->setClipRect(pOption->exposedRect);

    if (m_pComponent->m_pData->visible)
    {
        m_pComponent->Draw(pPainter, pOption);
    }


    if (m_pComponent->m_pData->selected)
    {
        qreal opacity = pPainter->opacity();
        pPainter->setOpacity(1.0);

        QPen pen(QColor(0xFF, 0xFF, 0x00, 0x88));
        pen.setWidth(13);
        pPainter->setPen(pen);
        pPainter->setBrush(Qt::NoBrush);
        pPainter->drawRect(m_boundingRect);

        pPainter->setOpacity(opacity);
    }
}


} // namespace BOI

