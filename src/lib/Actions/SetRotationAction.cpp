/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "Actions/SetRotationAction.h"


namespace BOI {


SetRotationAction::SetRotationAction()
    : Action(BOI_STD_A(SetRotation))
{
}


ActionCommand SetRotationAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    qreal rotation = 0.0;

    if ((pArgs != NULL) &&
        (pArgs->Contains("Rotation")))
    {
        rotation = pArgs->Value<qreal>("Rotation");
    }

    CRef cref = pSI->ActiveComponent();

    if (cref.IsValid())
    {
        pSI->SetRotation(cref, rotation);
    }

    return BOI_AC_STOP;
}


} // namespace BOI

