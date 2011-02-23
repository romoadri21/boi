/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SCROLLBOXACTION_H
#define __BOI_SCROLLBOXACTION_H


#include <QRectF>
#include "CRef.h"
#include "Action.h"
#include "Utilities/BoxInputHandler.h"


namespace BOI {


class ScrollBoxAction
    : public Action
{
    public:
        ScrollBoxAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

        void HandleViewTransformed(ASI* pSI);

        void Destroy();

    protected:
        void UpdateRectComponent(ASI* pSI);

    private:
        int m_numTouchStreams;

        QRectF m_innerViewRect;

        qreal m_x;
        qreal m_y;
        qreal m_startX;
        qreal m_startY;
        qreal m_scaleFactor;

        int  m_scrollerId;
        bool m_scrollerActive;
        qreal m_scrollVelocity;

        BoxInputHandler m_rect;

        CRef m_rectComponent;
        int m_setSizeReceiver;

        int* m_pErrorCode;
        QRectF* m_pTargetRect;
};


} // namespace BOI


#endif //__BOI_SCROLLBOXACTION_H

