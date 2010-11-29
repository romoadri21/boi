/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QSet>
#include <QList>
#include <QPointF>
#include <QStringList>
#include "ASI.h"
#include "UuidType.h"
#include "CRefList.h"
#include "ActionArgs.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Events/TouchEvent.h"
#include "Actions/InsertCollectionAction.h"


namespace BOI {


InsertCollectionAction::InsertCollectionAction()
    : Action(BOI_STD_A(InsertCollection))
{
}


ActionCommand InsertCollectionAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_uuid.clear();
    m_numTouchStreams = 0;

    ShowTextInputComponent(pSI);

    return BOI_AC_CONTINUE;
}


void InsertCollectionAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    if (!m_uuid.isEmpty())
    {
        pSI->UnsetCursor();
    }

    m_prevKeyEventHandler.Reset();
}


void InsertCollectionAction::Destroy()
{
    m_textInputComponent.Reset();
}


bool InsertCollectionAction::AcceptTouchStream()
{
    return (m_numTouchStreams == 0);
}


ActionCommand InsertCollectionAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        if (!m_uuid.isEmpty())
        {
            QPoint point(pEvent->x, pEvent->y);
            QPointF mappedPoint = pSI->MapFromViewToLayer(point);

            CRefList crefs = pSI->OpenCollection(m_uuid, mappedPoint);
            pSI->SetSelection(crefs);
        }

        m_numTouchStreams++;
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        m_numTouchStreams--;
    }

    return BOI_AC_CONTINUE;
}


void InsertCollectionAction::ShowTextInputComponent(ASI* pSI)
{
    if (!m_textInputComponent.IsValid() ||
         m_textInputComponent.IsDestroyed())
    {
        m_textInputComponent = pSI->NewComponent(BOI_STD_C(TextInput),
                                                 ViewLayerId_System);

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
            "Enter keywords that have been associated with a collection...";

        pSI->EmitTo(m_textInputComponent, m_setTextReceiver, dref);
    }

    m_prevKeyEventHandler = pSI->KeyEventHandler();

    pSI->SetKeyEventHandler(m_textInputComponent);
    pSI->SetVisible(m_textInputComponent, true);
}


ActionCommand InsertCollectionAction::Update(ASI* pSI, const ActionArgs* pArgs)
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
            QSet<QString> set;

            for (int i=0; i < numTags; i++)
            {
                QList<TypedUuid> uuids = pSI->GetTaggedUuids(parts.at(i).trimmed(),
                                                             UuidType_Collection);

                int numUuids = uuids.size();
                for (int j=0; j < numUuids; j++)
                {
                    set.insert(uuids.at(j).item1);
                }
            }

            if (!set.isEmpty())
            {
                m_uuid = set.toList().at(0);

                pSI->SetCursor(Qt::CrossCursor);

                return BOI_AC_CONTINUE;
            }
        }
    }

    return BOI_AC_STOP;
}


} // namespace BOI

