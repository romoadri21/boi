/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TOGGLEHOTSPOTACTION_H
#define __BOI_TOGGLEHOTSPOTACTION_H


#include "Action.h"


namespace BOI {


class ToggleHotSpotAction
    : public Action
{
    public:
        ToggleHotSpotAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);
};


} // namespace BOI


#endif //__BOI_TOGGLEHOTSPOTACTION_H

