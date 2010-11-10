/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_PASTEACTION_H
#define __BOI_PASTEACTION_H


#include <QString>
#include "Action.h"


namespace BOI {


class PasteAction
    : public Action
{
    public:
        PasteAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

    private:
        QString m_uuid;

        int m_numTouchStreams;
};


} // namespace BOI


#endif //__BOI_PASTEACTION_H

