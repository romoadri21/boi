/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COLLECTACTION_H
#define __BOI_COLLECTACTION_H


#include <QString>
#include "Action.h"
#include "CRefList.h"
#include "ActionArgs.h"


namespace BOI {


class CollectAction
    : public Action
{
    public:
        CollectAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool Suspend(ASI* pSI);

        ActionCommand Resume(ASI* pSI);

    private:
        CRefList m_crefs;

        QString m_text;
        int m_errorCode;
        ActionArgs m_args;
};


} // namespace BOI


#endif //__BOI_COLLECTACTION_H

