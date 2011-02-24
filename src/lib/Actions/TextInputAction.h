/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TEXTINPUTACTION_H
#define __BOI_TEXTINPUTACTION_H


#include "CRef.h"
#include "Action.h"


class QString;


namespace BOI {


class TextInputAction
    : public Action
{
    public:
        TextInputAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);
        void Destroy();

        ActionCommand Update(ASI* pSI, const ActionArgs* pArgs);

    private:
        CRef m_inputComponent;
        CRef m_prevKeyEventHandler;

        int m_funcSet;
        int m_setTextReceiver;
        int m_clearOnNextReceiver;

        bool m_componentActive;

        int* m_pErrorCode;
        QString* m_pText;
};


} // namespace BOI


#endif //__BOI_TEXTINPUTACTION_H

