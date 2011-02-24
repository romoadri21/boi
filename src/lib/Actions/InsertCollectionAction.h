/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_INSERTCOLLECTIONACTION_H
#define __BOI_INSERTCOLLECTIONACTION_H


#include <QString>
#include "CRef.h"
#include "Action.h"
#include "ActionArgs.h"


namespace BOI {


class InsertCollectionAction
    : public Action
{
    public:
        InsertCollectionAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

        bool Suspend(ASI* pSI);

        ActionCommand Resume(ASI* pSI);

    private:
        QString m_uuid;

        int m_numTouchStreams;

        QString m_text;
        int m_errorCode;
        ActionArgs m_args;
};


} // namespace BOI


#endif //__BOI_INSERTCOLLECTIONACTION_H

