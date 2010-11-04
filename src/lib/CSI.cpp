/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QApplication>
#include "State.h"
#include "MutexBase.h"
#include "Component.h"
#include "DataManager.h"
#include "ActionManager.h"
#include "ComponentData.h"
#include "ComponentManager.h"
#include "Events/SetActionEvent.h"
#include "Events/EventDispatcher.h"
#include "Events/UpdateActionEvent.h"
#include "Factories/CustomEventsFactory.h"
#include "CSI.h"


namespace BOI {


CSI::CSI(CustomEventsFactory* pCustomEventsFactory,
         ComponentManager*    pComponentManager,
         EventDispatcher*     pEventDispatcher,
         ActionManager*       pActionManager,
         DataManager*         pDataManager,
         MutexBase*           pMutexBase,
         State*               pState)
    : m_pCustomEventsFactory(pCustomEventsFactory),
      m_pComponentManager(pComponentManager),
      m_pEventDispatcher(pEventDispatcher),
      m_pActionManager(pActionManager),
      m_pDataManager(pDataManager),
      m_pMutexBase(pMutexBase),
      m_pState(pState)
{
}


void CSI::RegisterMutex(Mutex* pMutex)
{
    m_pMutexBase->Attach(pMutex);
}


int CSI::ConvertUuid_A(const QString& uuid)
{
    return m_pActionManager->ConvertUuid(uuid);
}


int CSI::ConvertUuid_C(const QString& uuid)
{
    return m_pComponentManager->ConvertUuid(uuid);
}


int CSI::ConvertUuid_D(const QString& uuid)
{
    return m_pDataManager->ConvertUuid(uuid);
}


DRef CSI::NewData(int type)
{
    return m_pDataManager->GetData(type);
}


DRef CSI::ImportData(QDataStream& in)
{
    return m_pDataManager->Import(in);
}


void CSI::ExportData(DRef& dref, QDataStream& out)
{
    m_pDataManager->Export(dref, out);
}


bool CSI::GetState(StateId stateId, QVariant& value)
{
    return m_pState->Get(stateId, value);
}


bool CSI::SetState(StateId stateId, const QVariant& value)
{
    return m_pState->Set(stateId, value);
}


void CSI::RegisterStateListener(StateId stateId, Component* pComponent, QVariant& value)
{
    m_pState->RegisterListener(stateId, pComponent->m_pData->cref, value);
}


void CSI::UnregisterStateListener(StateId stateId, Component* pComponent)
{
    m_pState->UnregisterListener(stateId, pComponent->m_pData->cref);
}


void CSI::SetActiveAction(int action, const ActionArgs* pArgs)
{
    SetActionEvent* pEvent = m_pCustomEventsFactory->NewSetActionEvent();
    pEvent->action = action;
    pEvent->pArgs = pArgs;

    QApplication::postEvent(m_pEventDispatcher, pEvent);
}


void CSI::UpdateActiveAction(int action, const ActionArgs* pArgs)
{
    UpdateActionEvent* pEvent = m_pCustomEventsFactory->NewUpdateActionEvent();
    pEvent->action = action;
    pEvent->pArgs = pArgs;

    QApplication::postEvent(m_pEventDispatcher, pEvent);
}


} // namespace BOI

