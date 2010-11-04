/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_DELETEACTION_H
#define __BOI_DELETEACTION_H


#include "Action.h"


namespace BOI {


class DeleteAction
    : public Action
{
    public:
        DeleteAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);
};


} // namespace BOI


#endif //__BOI_DELETEACTION_H

