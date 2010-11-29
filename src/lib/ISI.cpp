/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "MutexBase.h"
#include "DataManager.h"
#include "CRefListPool.h"
#include "TaskProcessor.h"
#include "ComponentManager.h"
#include "GuiRequestHandler.h"
#include "ISI.h"


namespace BOI {


ISI::ISI(GuiRequestHandler* pGuiRequestHandler,
         ComponentManager*  pComponentManager,
         TaskProcessor*     pTaskProcessor,
         CRefListPool*      pCRefListPool,
         DataManager*       pDataManager,
         MutexBase*         pMutexBase)
    : m_pGuiRequestHandler(pGuiRequestHandler),
      m_pComponentManager(pComponentManager),
      m_pTaskProcessor(pTaskProcessor),
      m_pCRefListPool(pCRefListPool),
      m_pDataManager(pDataManager),
      m_pMutexBase(pMutexBase)
{
}


CRef ISI::NewComponent(int type, ViewLayerId layer)
{
    return m_pComponentManager->NewComponent(type, layer);
}


DRef ISI::NewData(int type)
{
    return m_pDataManager->GetData(type);
}


QString ISI::GetUuid_C(int type)
{
    return m_pComponentManager->GetUuid(type);
}


QString ISI::GetUuid_D(int type)
{
    return m_pDataManager->GetUuid(type);
}


int ISI::ConvertUuid_C(const QString& uuid)
{
    return m_pComponentManager->ConvertUuid(uuid);
}


int ISI::ConvertUuid_D(const QString& uuid)
{
    return m_pDataManager->ConvertUuid(uuid);
}


DRef ISI::ImportData(QDataStream& in)
{
    return m_pDataManager->Import(in);
}


void ISI::ExportData(DRef& dref, QDataStream& out)
{
    m_pDataManager->Export(dref, out);
}


void ISI::RegisterMutex(Mutex* pMutex)
{
    m_pMutexBase->Attach(pMutex);
}


bool ISI::PostRequest(GuiRequest* pRequest)
{
    return m_pGuiRequestHandler->PostRequest(pRequest);
}


void ISI::ActivateComponent(ComponentData* pComponentData)
{
    m_pTaskProcessor->ActivateComponent(pComponentData);
}


void ISI::PostKeyEventTask(CRef cref, KeyEvent* pKeyEvent)
{
    m_pTaskProcessor->PostKeyEventTask(cref, pKeyEvent);
}


void ISI::PostTouchEventTask(CRef cref, TouchEvent* pTouchEvent)
{
    m_pTaskProcessor->PostTouchEventTask(cref, pTouchEvent);
}


void ISI::PostStateChangedTask(CRef cref, StateId stateId, DRef& dref)
{
    m_pTaskProcessor->PostStateChangedTask(cref, stateId, dref);
}


void ISI::PostEmitterConnectedTask(CRef cref, int emitter, int componentId)
{
    m_pTaskProcessor->PostEmitterConnectedTask(cref, emitter, componentId);
}


void ISI::PostEmitTask(int source, CRef cref, ReceiverFunc func, DRef& dref, bool release)
{
    m_pTaskProcessor->PostEmitTask(source, cref, func, dref, release);
}


void ISI::FreeTasks(ComponentData* pComponentData)
{
    m_pTaskProcessor->FreeTasks(pComponentData);
}


CRefList ISI::NewCRefList()
{
    return m_pCRefListPool->GetList();
}


void ISI::DestroyComponent(ComponentData* pComponentData)
{
    m_pComponentManager->DestroyComponent(pComponentData);
}


void ISI::DeleteComponent(ComponentData* pComponentData)
{
    m_pComponentManager->DeleteComponent(pComponentData);
}


} // namespace BOI

