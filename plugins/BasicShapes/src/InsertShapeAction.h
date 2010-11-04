/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __INSERTSHAPEACTION_H
#define __INSERTSHAPEACTION_H


#include "Action.h"


/*
 * Arguments:
 * ----------
 *  UUID = The UUID of the component to insert.
 */


class InsertShapeAction
    : public BOI::Action
{
    public:
        InsertShapeAction(int type);

        BOI::ActionCommand Start(BOI::ASI* pSI, const BOI::ActionArgs* pArgs);

        void Stop(BOI::ASI* pSI);

        bool AcceptTouchStream();

        BOI::ActionCommand HandleTouchEvent(BOI::ASI* pSI, BOI::TouchEvent* pEvent);

    private:
        int m_typeId;
        int m_numTouchStreams;
};


#endif //__INSERTSHAPEACTION_H

