/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QStringList>
#include "ASI.h"
#include "UuidType.h"
#include "StandardActions.h"
#include "Actions/CollectAction.h"


namespace BOI {


CollectAction::CollectAction()
    : Action(BOI_STD_A(Collect))
{
    m_args.SetAutoDelete(false);
    m_args.SetPtr("TextPointer", &m_text);
    m_args.SetPtr("ErrorCode", &m_errorCode);
    m_args.Set("InitialText", "Enter keywords to associate with the new collection...");
    m_args.Set("ClearOnNextPress", true);
}


ActionCommand CollectAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    CRef cref = pSI->ActiveComponent();

    if (!cref.IsValid() || cref.IsDestroyed())
    {
        return BOI_AC_STOP;
    }

    if (pSI->IsSelected(cref))
    {
        m_crefs = pSI->Selection();
    }
    else
    {
        m_crefs = pSI->NewCRefList();
        m_crefs.Append(cref);
    }

    return BOI_AC_SET(BOI_STD_A(TextInput), &m_args);
}


void CollectAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_crefs = CRefList();
}


bool CollectAction::Suspend(ASI* pSI)
{
    Q_UNUSED(pSI);

    return true;
}


ActionCommand CollectAction::Resume(ASI* pSI)
{
    if ((m_errorCode == 0) && !m_text.isEmpty())
    {
        QStringList parts = m_text.split(' ', QString::SkipEmptyParts);

        int numTags = parts.size();
        if (numTags > 0)
        {
            QString uuid = pSI->CreateCollection(m_crefs);

            if (!uuid.isEmpty())
            {
                for (int i=0; i < numTags; i++)
                {
                    pSI->AddTag(parts.at(i).trimmed(),
                                uuid,
                                UuidType_Collection);
                }
            }
        }
    }

    return BOI_AC_STOP;
}


} // namespace BOI

