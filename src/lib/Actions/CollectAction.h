/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COLLECTACTION_H
#define __BOI_COLLECTACTION_H


#include "CRef.h"
#include "Action.h"
#include "CRefList.h"


namespace BOI {


class CollectAction
    : public Action
{
    public:
        CollectAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);
        void Destroy();

        ActionCommand Update(ASI* pSI, const ActionArgs* pArgs);

    private:
        void ShowTextInputComponent(ASI* pSI);

    private:
        CRefList m_crefs;

        int  m_setTextReceiver;
        int  m_clearOnNextReceiver;
        CRef m_textInputComponent;
        CRef m_prevKeyEventHandler;
};


} // namespace BOI


#endif //__BOI_COLLECTACTION_H

