/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_GRAPHICSITEM_H
#define __BOI_GRAPHICSITEM_H


#include <QGraphicsItem>


namespace BOI {


class Component;


class GraphicsItem
    : public QGraphicsItem
{
    public:
        GraphicsItem(Component* pComponent);

        Component* GetComponent();

        void SetBoundingRect(const QRectF& rect);
        QRectF boundingRect() const;

        bool contains(const QPointF& point) const;

        void paint(QPainter* pPainter,
                   const QStyleOptionGraphicsItem* pOption,
                   QWidget* pWidget = 0);

        QPointF LayerPos() const;
        QRectF LayerBoundingRect() const;

    private:
        Component* m_pComponent;
        QRectF     m_boundingRect;
};


inline Component* GraphicsItem::GetComponent()
{
    return m_pComponent;
}


} // namespace BOI


#endif //__BOI_GRAPHICSITEM_H

