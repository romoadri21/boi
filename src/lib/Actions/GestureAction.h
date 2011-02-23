/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_GESTUREACTION_H
#define __BOI_GESTUREACTION_H


#include "CRef.h"
#include "Action.h"


namespace BOI {


class GestureAction
    : public Action
{
    public:
        GestureAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);
        ActionCommand HandleKeyEvent(ASI* pSI, KeyEvent* pEvent);

        void Destroy();

    private:
        int Quadrant(double x, double y);

    private:
        int m_numTouchStreams;

        double m_x1;
        double m_y1;
        double m_radius;

        CRef m_xgestureComponent;
};


} // namespace BOI


#endif //__BOI_GESTUREACTION_H

