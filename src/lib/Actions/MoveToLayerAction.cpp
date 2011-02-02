/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "Actions/MoveToLayerAction.h"


namespace BOI {


MoveToLayerAction::MoveToLayerAction()
    : Action(BOI_STD_A(MoveToLayer))
{
}


ActionCommand MoveToLayerAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    ViewLayerId layerId = ViewLayerId_Main;

    if ((pArgs != NULL) &&
        (pArgs->Contains("Layer")))
    {
        layerId = (ViewLayerId)pArgs->Value<int>("Layer");
    }
    else
    {
        return BOI_AC_STOP;
    }

    CRef cref = pSI->ActiveComponent();

    if (cref.IsValid())
    {
        pSI->MoveToLayer(cref, layerId);
    }

    return BOI_AC_STOP;
}


} // namespace BOI

