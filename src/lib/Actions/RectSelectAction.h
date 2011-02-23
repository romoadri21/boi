/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_RECTSELECTACTION_H
#define __BOI_RECTSELECTACTION_H


#include <QRectF>
#include "Action.h"
#include "ActionArgs.h"


namespace BOI {


class RectSelectAction
    : public Action
{
    public:
        RectSelectAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        bool Suspend(ASI* pSI);

        ActionCommand Resume(ASI* pSI);

    private:
        QRectF m_rect;

        int m_errorCode;
        ActionArgs m_args;
};


} // namespace BOI


#endif //__BOI_RECTSELECTACTION_H

