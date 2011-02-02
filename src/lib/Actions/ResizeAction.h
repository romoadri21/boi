/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_RESIZEACTION_H
#define __BOI_RESIZEACTION_H


#include "CRef.h"
#include "Action.h"


namespace BOI {


class ResizeAction
    : public Action
{
    public:
        ResizeAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptKeyStream();
        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);
        ActionCommand HandleKeyEvent(ASI* pSI, KeyEvent* pEvent);

        ActionCommand Update(ASI* pSI, const ActionArgs* pArgs);

        void Destroy();

    private:
        enum
        {
            Axis_X = 0,
            Axis_Y,
            Axis_XY
        };

    private:
        int m_numTouchStreams;

        int m_axis;

        qreal m_xPrev;
        qreal m_yPrev;

        CRef m_cref;
        int m_setSizeReceiver;
        int m_updateSizeReceiver;

        bool m_textInputActive;
        int  m_setTextReceiver;
        CRef m_textInputComponent;

        CRef m_prevKeyEventHandler;
};


} // namespace BOI


#endif //__BOI_RESIZEACTION_H

