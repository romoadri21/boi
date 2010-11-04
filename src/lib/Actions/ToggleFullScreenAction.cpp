/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Actions/ToggleFullScreenAction.h"


namespace BOI {


ToggleFullScreenAction::ToggleFullScreenAction()
    : Action(BOI_STD_A(ToggleFullScreen))
{
}


ActionCommand ToggleFullScreenAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    pSI->ToggleFullScreen();

    return BOI_AC_STOP;
}


} // namespace BOI

