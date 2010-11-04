/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_REMOVEFROMLAYOUTACTION_H
#define __BOI_REMOVEFROMLAYOUTACTION_H


#include "CRef.h"
#include "Action.h"


namespace BOI {


class RemoveFromLayoutAction
    : public Action
{
    public:
        RemoveFromLayoutAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

    private:
        int m_numTouchStreams;

        CRef m_layout;
};


} // namespace BOI


#endif //__BOI_REMOVEFROMLAYOUTACTION_H

