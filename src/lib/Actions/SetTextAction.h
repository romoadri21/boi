/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SETTEXTACTION_H
#define __BOI_SETTEXTACTION_H


#include "CRef.h"
#include "Action.h"
#include "Utilities/TextInputWrapper.h"


namespace BOI {


class SetTextAction
    : public Action
{
    public:
        SetTextAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);
        void Destroy();

        ActionCommand Update(ASI* pSI, const ActionArgs* pArgs);

    private:
        CRef m_activeComponent;
        CRef m_prevKeyEventHandler;

        TextInputWrapper m_inputComponent;
};


} // namespace BOI


#endif //__BOI_SETTEXTACTION_H

