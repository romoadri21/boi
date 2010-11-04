/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QVariant>
#include <QSizeF>
#include "ASI.h"
#include "StateId.h"
#include "StandardActions.h"
#include "Actions/ResetViewScaleAction.h"


namespace BOI {


ResetViewScaleAction::ResetViewScaleAction()
    : Action(BOI_STD_A(ResetViewScale))
{
}


ActionCommand ResetViewScaleAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    QVariant value;
    pSI->GetState(StateId_WindowSize, value);
    QSizeF size = value.toSizeF();
    size *= 0.5;

    QPoint viewCenter = QPoint(size.width(), size.height());
    QPointF centerPoint = pSI->MapFromViewToScene(viewCenter);

    pSI->ResetViewTransform();
    pSI->CenterViewOn(centerPoint);

    return BOI_AC_STOP;
}


} // namespace BOI

