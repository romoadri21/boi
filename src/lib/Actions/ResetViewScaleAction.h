/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_RESETVIEWSCALEACTION_H
#define __BOI_RESETVIEWSCALEACTION_H


#include "Action.h"


namespace BOI {


class ResetViewScaleAction
    : public Action
{
    public:
        ResetViewScaleAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

    private:
        int m_numTouchStreams;
};


} // namespace BOI


#endif //__BOI_RESETVIEWSCALEACTION_H

