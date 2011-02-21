/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ZOOMTORECTACTION_H
#define __BOI_ZOOMTORECTACTION_H


#include <QRectF>
#include "Action.h"
#include "ActionArgs.h"


namespace BOI {


class ZoomToRectAction
    : public Action
{
    public:
        ZoomToRectAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        bool Suspend(ASI* pSI);

        ActionCommand Resume(ASI* pSI);

    private:
        QRectF m_rect;
        ActionArgs m_args;
};


} // namespace BOI


#endif //__BOI_ZOOMTORECTACTION_H

