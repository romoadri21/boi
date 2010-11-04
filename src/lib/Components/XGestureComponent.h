/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_XGESTURECOMPONENT_H
#define __BOI_XGESTURECOMPONENT_H


#include <QRectF>
#include <QImage>
#include "Component.h"


namespace BOI {


class XGestureComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    public:
        XGestureComponent();

    protected:
        virtual bool Initialize();

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

    private:
        QRectF m_boundingRect;
        QImage m_image;

        float m_radius;
};


} // namespace BOI


#endif //__BOI_XGESTURECOMPONENT_H

