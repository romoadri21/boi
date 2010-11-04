/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MOVEACTION_H
#define __BOI_MOVEACTION_H


#include "CRef.h"
#include "Action.h"


namespace BOI {


class MoveAction
    : public Action
{
    public:
        MoveAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

    private:
        int m_numTouchStreams;

        CRef m_cref;

        qreal m_xDelta;
        qreal m_yDelta;
};


} // namespace BOI


#endif //__BOI_MOVEACTION_H

