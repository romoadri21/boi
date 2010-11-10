/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QString>
#include <QList>
#include "ASI.h"
#include "CRef.h"
#include "CRefList.h"
#include "UuidType.h"
#include "StandardActions.h"
#include "Actions/CopyAction.h"


#define BOI_CLIPBOARD_TAG "BOI.Clipboard"


namespace BOI {


CopyAction::CopyAction()
    : Action(BOI_STD_A(Copy))
{
}


ActionCommand CopyAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    CRef cref = pSI->ActiveComponent();

    if (!cref.IsValid() || cref.IsDestroyed())
    {
        return BOI_AC_STOP;
    }


    CRefList crefs;

    if (pSI->IsSelected(cref))
    {
        crefs = pSI->Selection();
    }
    else
    {
        crefs = pSI->NewCRefList();
        crefs.Append(cref);
    }


    QString uuid;
    QList<TypedUuid> uuids = pSI->GetTaggedUuids(BOI_CLIPBOARD_TAG,
                                                 UuidType_Collection);

    if (!uuids.isEmpty())
    {
        uuid = uuids.at(0).item1;

        pSI->DeleteCollection(uuid);
    }

    uuid = pSI->CreateCollection(crefs, uuid);

    if (uuids.isEmpty())
    {
        pSI->AddTag(BOI_CLIPBOARD_TAG, uuid, UuidType_Collection);
    }

    return BOI_AC_STOP;
}


} // namespace BOI

