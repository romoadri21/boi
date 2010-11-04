/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "CRef.h"
#include "CRefList.h"
#include "StandardActions.h"
#include "Actions/DeleteAction.h"


namespace BOI {


DeleteAction::DeleteAction()
    : Action(BOI_STD_A(Delete))
{
}


ActionCommand DeleteAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    CRef cref = pSI->ActiveComponent();

    if (cref.IsValid())
    {
        if (pSI->IsSelected(cref))
        {
            CRefList crefs = pSI->Selection();

            int numCRefs = crefs.Count();
            for (int i=0; i < numCRefs; i++)
            {
                cref = crefs.Value(i);

                if (cref.IsValid())
                {
                    cref.DestroyInstance();
                }
            }

            /*
             * Clear the selection.
             */
            crefs = CRefList();
            pSI->SetSelection(crefs);
        }
        else
        {
            cref.DestroyInstance();
        }
    }

    return BOI_AC_STOP;
}


} // namespace BOI

