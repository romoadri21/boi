/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "Action.h"


namespace BOI {


ActionCommand Action::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pArgs);

    return BOI_AC_CONTINUE;
}


void Action::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);
}


bool Action::Suspend(ASI* pSI)
{
    Q_UNUSED(pSI);

    return false;
}


ActionCommand Action::Resume(ASI* pSI)
{
    Q_UNUSED(pSI);

    return BOI_AC_STOP;
}


bool Action::AcceptTouchStream()
{
    return false;
}


bool Action::AcceptKeyStream()
{
    return false;
}


ActionCommand Action::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pEvent);

    return BOI_AC_CONTINUE;
}


ActionCommand Action::HandleKeyEvent(ASI* pSI, KeyEvent* pEvent)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pEvent);

    return BOI_AC_CONTINUE;
}


ActionCommand Action::Update(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pArgs);

    return BOI_AC_CONTINUE;
}


void Action::HandleViewTransformed(ASI* pSI)
{
    Q_UNUSED(pSI);
}


void Action::Destroy()
{
}


} // namespace BOI

