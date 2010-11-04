/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "StandardActions.h"
#include "Actions/NullAction.h"


namespace BOI {


NullAction::NullAction()
    : Action(BOI_STD_A(Null))
{
}


ActionCommand NullAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pArgs);

    return BOI_AC_STOP;
}


} // namespace BOI

