/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Actions/ClearSelectionAction.h"


namespace BOI {


ClearSelectionAction::ClearSelectionAction()
    : Action(BOI_STD_A(ClearSelection))
{
}


ActionCommand ClearSelectionAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    CRefList crefs;
    pSI->SetSelection(crefs);

    return BOI_AC_STOP;
}


} // namespace BOI

