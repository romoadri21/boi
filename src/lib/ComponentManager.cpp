/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "ISI.h"
#include "Component.h"
#include "GuiRequest.h"
#include "ComponentData.h"
#include "StandardComponents.h"
#include "ComponentManager.h"


namespace BOI {


ComponentManager::ComponentManager()
    : FactoryRegistrar(),
      m_typeManager(),
      m_pCSI(NULL),
      m_pISI(NULL),
      m_componentId(0),
      m_numComponents(0),
      m_numNewAccessors(0),
      MaxNewAccessors(0x10000),
      m_mutex(),
      m_pHead(NULL)
{
}


ComponentManager::~ComponentManager()
{
    Q_ASSERT(m_numComponents == 0);
}


void ComponentManager::SetCSI(CSI* pCSI)
{
    m_pCSI = pCSI;
}


void ComponentManager::SetISI(ISI* pISI)
{
    m_pISI = pISI;
}


bool ComponentManager::Initialize()
{
    /*
     * Reserve space and the matching system internal
     * type for the built-in Components.
     */

    for (int i=0; i < StandardComponents::NumComponents; i++)
    {
        m_typeManager.RegisterType(NULL,
                                   NULL,
                                   NULL,
                                   StandardComponents::Uuid(i));
    }

    return true;
}


void ComponentManager::RegisterFactory(Factory* pFactory)
{
    pFactory->RegisterTypes(&m_typeManager);
}


CRef ComponentManager::NewComponent(int type, ViewLayerId layer)
{
    CRef cref;

    if (!m_typeManager.TypeRegistered(type))
    {
        /*
         * The caller is requesting a type that is not registered
         * with the system. Return an invalid CRef.
         */
        return cref;
    }

    int numAccessors = m_numNewAccessors.fetchAndAddOrdered(1);
    if (numAccessors < MaxNewAccessors)
    {
        Component* pComponent = (Component*)m_typeManager.GetInstance(type);
        ComponentData* pData = pComponent->m_pData;

        cref = CRef(pData);

        m_numComponents.ref();

        AddToList(pData);

        pData->cref = cref;
        pData->pCSI = m_pCSI;
        pData->pISI = m_pISI;
        pData->id = m_componentId.fetchAndAddRelaxed(1);

        m_pISI->RegisterMutex(&pData->crefLock);
        m_pISI->RegisterMutex(&pData->drawLock);
        m_pISI->RegisterMutex(&pData->tasksLock);

        /*
         * Lock the component so that it can not be destroyed
         * while it's being initialized. This can potentially
         * happen since CRef::DestroyInstance() could be called
         * from within Component::Initialize() or in code that
         * has received a CRef from within Component::Initialize().
         */
        cref.GetInstance();

        pData->connections.Initialize(m_pISI, pComponent->IFace(), pData->id);

        pComponent->Initialize();

        /*
         * Activate task processing for
         * the component.
         */
        m_pISI->ActivateComponent(pData);

        cref.ReleaseInstance();

        GuiRequest request;
        request.type = GuiRequest::RequestType_MoveToLayer;
        request.cref = cref;
        request.data.viewLayerId = layer;
        m_pISI->PostRequest(&request);
    }

    m_numNewAccessors.fetchAndAddOrdered(-1);
    return cref;
}


void ComponentManager::DestroyAll(bool stopNew)
{
    /* 
     * Stop the rest of the application from being able
     * to add Objects. Wait until all threads that are
     * currently in NewComponent (if any) to leave the method.
     * Note, the polling method of waiting used here can lead
     * to a situation where the while loop never returns if
     * threads continuously try to get new components (even when
     * its not possible to get more components). This situation
     * will most likely not occur frequently (if at all)
     * so it should be o.k. to leave the code as is.
     */
    m_numNewAccessors.fetchAndAddOrdered(MaxNewAccessors);

    while (m_numNewAccessors.fetchAndAddOrdered(0) !=
           MaxNewAccessors) {};


    m_mutex.lock();

    ComponentData* pComponentData = m_pHead;
    while (pComponentData != NULL)
    {
        /*
         * Only call DestroyInstance() if the CRef
         * has not already been Reset. If the CRef
         * has been Reset(), which only happens in
         * GuiRequestHandler::DestroyComponent(),
         * then the referenced component has already
         * been destroyed and has not been deleted
         * yet because there is still a CRef that
         * is referencing the component (ie. a key
         * event handler which is destroyed right
         * before the application shuts down).
         * NOTE: this method should only be called
         * from the main gui thread (the same thread
         * as the GuiRequestHandler).
         */
        if (pComponentData->cref.IsValid())
        {
            pComponentData->cref.DestroyInstance();
        }

        pComponentData = pComponentData->pNext;
    }

    m_mutex.unlock();


    if (!stopNew)
    {
        m_numNewAccessors.fetchAndAddOrdered(-MaxNewAccessors);
    }
}


void ComponentManager::DestroyComponent(ComponentData* pComponentData)
{
    m_pISI->FreeTasks(pComponentData);

    GuiRequest request;
    request.type = GuiRequest::RequestType_DestroyComponent;
    request.data.pComponentData = pComponentData;
    m_pISI->PostRequest(&request);
}


void ComponentManager::DeleteComponent(ComponentData* pComponentData)
{
    /*
     * This call was initially the first line in DestroyComponent(...)
     * but it was moved here to avoid the need for a recursive mutex
     * since DestroyAll()=>DestroyInstance(...) could end up calling
     * DestroyComponent(...) while still inside the mutex lock.
     */
    RemoveFromList(pComponentData);

    m_typeManager.DeleteInstance(pComponentData->pComponent);

    m_numComponents.deref();
}


void ComponentManager::AddToList(ComponentData* pComponentData)
{
    m_mutex.lock();

    if (m_pHead != NULL)
    {
        pComponentData->pNext = m_pHead;
        m_pHead->pPrev = pComponentData;
    }

    m_pHead = pComponentData;

    m_mutex.unlock();
}


void ComponentManager::RemoveFromList(ComponentData* pComponentData)
{
    m_mutex.lock();

    if (pComponentData->pPrev != NULL)
    {
        pComponentData->pPrev->pNext = pComponentData->pNext;
    }
    else
    {
        m_pHead = pComponentData->pNext;
    }

    if (pComponentData->pNext != NULL)
    {
        pComponentData->pNext->pPrev = pComponentData->pPrev;
    }

    m_mutex.unlock();
}


} // namespace BOI

