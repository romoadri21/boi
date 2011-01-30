/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SETNUMBERACTION_H
#define __BOI_SETNUMBERACTION_H


#include "CRef.h"
#include "Action.h"


/*
 * Arguments:
 * ----------
 *  Receiver = The UUID of the receiver to emit value to.
 *  MinValue = The minimum value for the number.
 *  MaxValue = The maximum value for the number.
 *  MinDelta = The smallest step size when stepping through values.
 *  MaxDelta = The largest step size when stepping through values.
 *  Value    = The initial value.
 *  Interactive = A boolean value that when true allows the user to
 *                interactively modify the value. When set to false,
 *                the value is set when the action starts and the
 *                action immediately stops.
 */


namespace BOI {


class SetNumberAction
    : public Action
{
    public:
        SetNumberAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

    protected:
        void SetValue(ASI* pSI, double value);

    private:
        int m_numTouchStreams;

        double m_value;
        double m_minValue;
        double m_maxValue;
        double m_minDelta;
        double m_maxDelta;

        double m_xPrev;
        double m_yPrev;

        double m_xStep;
        double m_yStep;

        int m_receiver;
        CRef m_activeComponent;
};


} // namespace BOI


#endif //__BOI_SETNUMBERACTION_H

