/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_LOADURLACTION_H
#define __BOI_LOADURLACTION_H


#include "CRef.h"
#include "Action.h"


namespace BOI {


class LoadUrlAction
    : public Action
{
    public:
        LoadUrlAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);
        void Destroy();

        ActionCommand Update(ASI* pSI, const ActionArgs* pArgs);

    private:
        void ShowTextInputComponent(ASI* pSI);

    private:
        CRef m_activeComponent;

        int  m_setTextReceiver;
        int  m_clearOnNextReceiver;
        CRef m_textInputComponent;
        CRef m_prevKeyEventHandler;
};


} // namespace BOI


#endif //__BOI_LOADURLACTION_H

