/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_BOXINPUTHANDLER_H
#define __BOI_BOXINPUTHANDLER_H


#include <QPointF>
#include <QRectF>
#include "BOILib.h"


namespace BOI {


class BOI_LIB_EXPORT BoxInputHandler
{
    public:
        BoxInputHandler();

        void SetStartPoint(const QPointF& startPoint);
        void SetEndPoint(const QPointF& endPoint);

        const QRectF& Rect();

    private:
        QPointF m_startPoint;
        QRectF  m_rect;
};


} // namespace BOI


#endif //__BOI_BOXINPUTHANDLER_H

