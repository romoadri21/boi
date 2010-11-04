/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QString>
#include <QStringList>
#include "ASI.h"
#include "UuidType.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Actions/CollectAction.h"


namespace BOI {


CollectAction::CollectAction()
    : Action(BOI_STD_A(Collect))
{
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

    ShowTextInputComponent(pSI);

    return BOI_AC_CONTINUE;
}


void CollectAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    m_crefs = CRefList();
    m_prevKeyEventHandler.Reset();
}


void CollectAction::Destroy()
{
    m_textInputComponent.Reset();
}


void CollectAction::ShowTextInputComponent(ASI* pSI)
{
    if (!m_textInputComponent.IsValid() ||
         m_textInputComponent.IsDestroyed())
    {
        m_textInputComponent = pSI->NewComponent(BOI_STD_C(TextInput),
                                                 SceneLayerId_System);

        int funcSet = pSI->GetFuncSet(m_textInputComponent,
                                      "{790e6f3f-4433-4490-a141-a4cb4433b0e7}");
        if (funcSet != -1)
        {
            /*
             * Set the Action type id so that
             * Update gets called correctly.
             */

            DRef dref = pSI->NewData(BOI_STD_D(Int));
            *dref.GetWriteInstance<int>() = Type();
            pSI->CallFunc(m_textInputComponent, funcSet, 0, dref);
        }

        m_clearOnNextReceiver = pSI->GetReceiver(m_textInputComponent,
                                               "{f58a0c2e-63c7-41e4-8895-6c7fa44ae32f}");

        m_setTextReceiver = pSI->GetReceiver(m_textInputComponent,
                                             "{7f2249e4-6c3c-40a5-9cff-59501f06ee37}");
    }

    if (m_clearOnNextReceiver != -1)
    {
        /*
         * Tell the component to remove the
         * text after the next key press.
         */

        DRef dref = pSI->NewData(BOI_STD_D(Bool));
        *dref.GetWriteInstance<bool>() = true;

        pSI->EmitTo(m_textInputComponent, m_clearOnNextReceiver, dref);
    }

    if (m_setTextReceiver != -1)
    {
        DRef dref = pSI->NewData(BOI_STD_D(String));
        *dref.GetWriteInstance<QString>() =
            "Enter keywords to associate with the new collection...";

        pSI->EmitTo(m_textInputComponent, m_setTextReceiver, dref);
    }

    m_prevKeyEventHandler = pSI->KeyEventHandler();

    pSI->SetKeyEventHandler(m_textInputComponent);
    pSI->SetVisible(m_textInputComponent, true);
}


ActionCommand CollectAction::Update(ASI* pSI, const ActionArgs* pArgs)
{
    pSI->SetVisible(m_textInputComponent, false);
    pSI->SetKeyEventHandler(m_prevKeyEventHandler);

    if ((pArgs != NULL) &&
        (pArgs->Contains("Text")))
    {
        QString text = pArgs->Value<QString>("Text");
        QStringList parts = text.split(' ', QString::SkipEmptyParts);

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

