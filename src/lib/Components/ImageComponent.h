/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __IMAGECOMPONENT_H
#define __IMAGECOMPONENT_H


#include <QRectF>
#include <QImage>
#include <QTransform>
#include "Component.h"


namespace BOI {


class ImageComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    BOI_DEFINE_RECEIVER_FUNC(ImageComponent, SetImage);

    BOI_DEFINE_RECEIVER_FUNC(ImageComponent, SetSize);
    BOI_DEFINE_RECEIVER_FUNC(ImageComponent, UpdateSize);

    public:
        ImageComponent();

    protected:
        virtual bool Initialize();

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

        virtual void Import(const QHash<int, BOI::DRef>& in);
        virtual void Export(QHash<int, BOI::DRef>& out);

    private:
        void UpdateTransform();

    private:
        QRectF m_rect;
        QImage m_image;
        QTransform m_transform;
};


} // namespace BOI


#endif //__IMAGECOMPONENT_H

