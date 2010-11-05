/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Actions/FitAllInViewAction.h"


namespace BOI {


FitAllInViewAction::FitAllInViewAction()
    : Action(BOI_STD_A(FitAllInView))
{
}


ActionCommand FitAllInViewAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    pSI->FitAllInView();

    return BOI_AC_STOP;
}


} // namespace BOI

