/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QImage>
#include "ASI.h"
#include "CRef.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Actions/CaptureViewAction.h"


namespace BOI {


CaptureViewAction::CaptureViewAction()
    : Action(BOI_STD_A(CaptureView))
{
    m_args.SetAutoDelete(false);
    m_args.SetPtr("RectPointer", &m_rect);
}


ActionCommand CaptureViewAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pSI);
    Q_UNUSED(pArgs);

    /*
     * Invalidate the rect.
     */
    m_rect.setWidth(0);

    return BOI_AC_SET(BOI_STD_A(ScrollBox), &m_args);
}


bool CaptureViewAction::Suspend(ASI* pSI)
{
    Q_UNUSED(pSI);

    return true;
}


bool CaptureViewAction::Resume(ASI* pSI)
{
    if (m_rect.isValid())
    {
        QImage image = pSI->CaptureRect(m_rect);

        CRef cref = pSI->NewComponent(BOI_STD_C(Image));

        int receiver = pSI->GetReceiver(cref, "{7d4269b3-db0b-43cd-8afc-bd5e8251ac36}");

        DRef dref = pSI->NewData(BOI_STD_D(Image));
        *dref.GetWriteInstance<QImage>() = image;

        pSI->EmitTo(cref, receiver, dref, true);

        pSI->SetVisible(cref, true);

        QPointF newPos = m_rect.topLeft();
        pSI->SetPosition(cref, newPos);
    }

    return false;
}


} // namespace BOI

