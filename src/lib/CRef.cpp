/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "ISI.h"
#include "Mutex.h"
#include "Component.h"
#include "ComponentData.h"
#include "StandardComponents.h"
#include "CRef.h"


namespace BOI {


CRef::CRef(ComponentData* pComponentData)
{
    m_pComponentData = pComponentData;

    m_pComponentData->refCount     = 1;
    m_pComponentData->needsDestroy = false;
}


CRef::CRef(const CRef& cref)
{
    m_pComponentData = cref.m_pComponentData;

    if (m_pComponentData != NULL)
    {
        /*
         * Mutex locking is not necessary here since there will
         * always be another CRef in existence (the one
         * passed in) and atomic operations are used.
         */
        m_pComponentData->refCount.ref();
    }
}


int CRef::Type() const
{
    return (m_pComponentData != NULL) ?
           (m_pComponentData->pComponent->Type()) :
           (BOI_STD_C(Invalid));
}


int CRef::Id() const
{
    return (m_pComponentData != NULL) ?
           (m_pComponentData->id) :
           (-1);
}


Component* CRef::GetInstance()
{
    m_pComponentData->crefLock.Lock();

    if (!m_pComponentData->needsDestroy)
    {
        m_pComponentData->instanceCount++;
        m_pComponentData->crefLock.Unlock();
        return m_pComponentData->pComponent;
    }

    m_pComponentData->crefLock.Unlock();

    return NULL;
}


void CRef::ReleaseInstance()
{
    bool destroyComponent = false;

    m_pComponentData->crefLock.Lock();
    m_pComponentData->instanceCount--;

    if ((m_pComponentData->instanceCount == 0) &&
        (m_pComponentData->needsDestroy))
    {
        destroyComponent = true;
    }

    m_pComponentData->crefLock.Unlock();

    if (destroyComponent)
    {
        m_pComponentData->pISI->DestroyComponent(m_pComponentData);
    }
}


void CRef::DestroyInstance()
{
    bool destroyComponent = false;

    m_pComponentData->crefLock.Lock();

    if (!m_pComponentData->needsDestroy)
    {
        if (m_pComponentData->instanceCount == 0)
        {
            destroyComponent = true;
        }

        m_pComponentData->needsDestroy = true;
    }

    m_pComponentData->crefLock.Unlock();

    if (destroyComponent)
    {
        m_pComponentData->pISI->DestroyComponent(m_pComponentData);
    }
}


bool CRef::IsDestroyed() const
{
    if (m_pComponentData != NULL)
    {
        return m_pComponentData->needsDestroy;
    }

    return true;
}


void CRef::Reset()
{
    if (m_pComponentData != NULL)
    {
        /*
         * Mutex locking is not necessary here because an atomic operation
         * is being used and thus only one thread will ever encounter
         * the refCount at 0. The thread that does encounter the refCount
         * at 0 can safely access the destroyState in the referenced component
         * since there are no other CRefs in existence to modify the
         * destroyState.
         */
        if (!m_pComponentData->refCount.deref())
        {
            /*
             * ComponentManager::DestroyComponent() should have already been
             * called before this code path is reached because of the CRef that
             * is stored in the Component by the ComponentManager.
             */
            Q_ASSERT(m_pComponentData->needsDestroy);

            m_pComponentData->pISI->DeleteComponent(m_pComponentData);
        }

        m_pComponentData = NULL;
    }
}


CRef& CRef::operator=(const CRef& cref)
{
    if (this != &cref)
    {
        Reset();

        m_pComponentData = cref.m_pComponentData;

        if (m_pComponentData != NULL)
        {
            /*
             * Mutex locking is not necessary here since there will
             * always be another CRef in existence (the one
             * passed in) and atomic operations are being used.
             */
            m_pComponentData->refCount.ref();
        }
    }

    return *this;
}


} // namespace BOI

