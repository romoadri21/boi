/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_HIDERACTION_H
#define __BOI_HIDERACTION_H


#include "CRef.h"
#include "Action.h"


/*
 * Arguments:
 * ----------
 *  CRef = The component to hide when this
 *         action is stopped or suspended.
 */


namespace BOI {


class HiderAction
    : public Action
{
    public:
        HiderAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

    private:
        CRef m_cref;
};


} // namespace BOI


#endif //__BOI_HIDERACTION_H

