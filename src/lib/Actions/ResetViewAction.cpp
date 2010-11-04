/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Actions/ResetViewAction.h"


namespace BOI {


ResetViewAction::ResetViewAction()
    : Action(BOI_STD_A(ResetView))
{
}


ActionCommand ResetViewAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    pSI->ResetViewTransform();
    pSI->CenterViewOn(QPointF(0, 0));

    return BOI_AC_STOP;
}


} // namespace BOI

