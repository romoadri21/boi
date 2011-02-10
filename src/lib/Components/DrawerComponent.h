/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __DRAWERCOMPONENT_H
#define __DRAWERCOMPONENT_H


#include <QPen>
#include <QFont>
#include <QBrush>
#include <QRectF>
#include <QPointF>
#include <QString>
#include "Component.h"


namespace BOI {


class DrawerComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();
    BOI_DEFINE_RECEIVER_FUNC(DrawerComponent, SetText);
    BOI_DEFINE_RECEIVER_FUNC(DrawerComponent, SetExtent);

    public:
        DrawerComponent();

    protected:
        virtual bool Initialize();

        virtual void HandleTouchEvent(TouchEvent* pEvent);

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

    private:
        void UpdateFullRect();
        void UpdateHandleRect();

    private:
        bool m_open;

        QFont m_font;
        QString m_label;

        QPointF m_extent;
        QRectF  m_fullRect;
        QRectF  m_handleRect;

        QBrush m_bgFill;

        QPen m_textPen;
        QPen m_separatorPen;
};


} // namespace BOI


#endif //__DRAWERCOMPONENT_H

