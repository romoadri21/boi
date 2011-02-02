/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __RECTCOMPONENT_H
#define __RECTCOMPONENT_H


#include <QPen>
#include <QBrush>
#include <QRectF>
#include "Component.h"


class RectComponent
    : public BOI::Component
{
    BOI_INTERFACE_DEFINITION();

    BOI_DEFINE_RECEIVER_FUNC(RectComponent, SetSize);
    BOI_DEFINE_RECEIVER_FUNC(RectComponent, UpdateSize);

    BOI_DEFINE_RECEIVER_FUNC(RectComponent, SetPen);
    BOI_DEFINE_RECEIVER_FUNC(RectComponent, SetBrush);
    BOI_DEFINE_RECEIVER_FUNC(RectComponent, SetRadii);

    BOI_DEFINE_RECEIVER_FUNC(RectComponent, SetBorderWidth);

    public:
        RectComponent(int type);

    protected:
        virtual bool Initialize();

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

        virtual void Import(const QHash<int, BOI::DRef>& in);
        virtual void Export(QHash<int, BOI::DRef>& out);

        void UpdateRect();

    private:
        QRectF m_rect;
        QRectF m_boundingRect;

        qreal m_xRadius;
        qreal m_yRadius;

        QPen   m_pen;
        QBrush m_brush;
};


#endif //__RECTCOMPONENT_H

