/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_PANACTION_H
#define __BOI_PANACTION_H


#include <QRectF>
#include "Action.h"
#include "Utilities/TouchVelocity.h"


namespace BOI {


class PanAction
    : public Action
{
    public:
        PanAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

    private:
        int m_numTouchStreams;

        qreal m_startX;
        qreal m_startY;
        qreal m_scaleFactor;

        QRectF m_innerViewRect;

        int  m_scrollerId;
        bool m_scrollerActive;

        qreal m_scrollVelocity;
        qreal m_kineticScrollAcceleration;
        qreal m_kineticScrollActivateVelocity;

        TouchVelocity m_touchVelocity;
};


} // namespace BOI


#endif //__BOI_PANACTION_H

