/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ASI.h"
#include "StandardActions.h"
#include "Actions/ZoomToRectAction.h"


namespace BOI {


ZoomToRectAction::ZoomToRectAction()
    : Action(BOI_STD_A(ZoomToRect))
{
    m_args.SetAutoDelete(false);
    m_args.SetPtr("RectPointer", &m_rect);
}


ActionCommand ZoomToRectAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pArgs);

    /*
     * Invalidate the rect.
     */
    m_rect.setWidth(0);

    return BOI_AC_SET(BOI_STD_A(ScrollBox), &m_args);
}


bool ZoomToRectAction::Suspend(ASI* pSI)
{
    Q_UNUSED(pSI);

    return true;
}


ActionCommand ZoomToRectAction::Resume(ASI* pSI)
{
    if (m_rect.isValid())
    {
        pSI->FitRectInView(m_rect);
    }

    return BOI_AC_STOP;
}


} // namespace BOI

