/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_XGESTUREACTION_H
#define __BOI_XGESTUREACTION_H


#include "CRef.h"
#include "Action.h"


namespace BOI {


class XGestureAction
    : public Action
{
    public:
        XGestureAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);
        ActionCommand HandleKeyEvent(ASI* pSI, KeyEvent* pEvent);

        void Destroy();

    private:
        int Quadrant(double x, double y);

        bool ShowMenu(ASI* pSI, const QPointF& centerPoint);

    private:
        int m_numTouchStreams;

        double m_x1;
        double m_y1;
        double m_radius;

        int m_menuFuncSet;

        CRef m_menuComponent;
        CRef m_xgestureComponent;
};


} // namespace BOI


#endif //__BOI_XGESTUREACTION_H

