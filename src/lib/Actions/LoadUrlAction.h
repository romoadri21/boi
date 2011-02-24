/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_LOADURLACTION_H
#define __BOI_LOADURLACTION_H


#include <QString>
#include "CRef.h"
#include "Action.h"
#include "ActionArgs.h"


namespace BOI {


class LoadUrlAction
    : public Action
{
    public:
        LoadUrlAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool Suspend(ASI* pSI);

        ActionCommand Resume(ASI* pSI);

    private:
        QString m_text;
        int m_errorCode;
        ActionArgs m_args;

        CRef m_activeComponent;
};


} // namespace BOI


#endif //__BOI_LOADURLACTION_H

