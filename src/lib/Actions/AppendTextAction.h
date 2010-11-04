/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_APPENDTEXTACTION_H
#define __BOI_APPENDTEXTACTION_H


#include "CRef.h"
#include "Action.h"


namespace BOI {


class AppendTextAction
    : public Action
{
    public:
        AppendTextAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

    private:
        int m_numTouchStreams;

        CRef m_target;
        int m_appendTextReceiver;
};


} // namespace BOI


#endif //__BOI_APPENDTEXTACTION_H

