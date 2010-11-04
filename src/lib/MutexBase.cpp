/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ThreadLockData.h"
#include "Mutex.h"
#include "MutexBase.h"


namespace BOI {


MutexBase::MutexBase()
    : m_mutexId(0),
      m_lockData()
{
}


void MutexBase::Attach(Mutex* pMutex)
{
    pMutex->m_pBase = this;
    pMutex->m_pMutex = &m_mutexes[m_mutexId];
    m_mutexId = (m_mutexId + 1) % BOI_MUTEXBASE_MAX_QMUTEXES;
}


ThreadLockData* MutexBase::GetData()
{
    ThreadLockData* pThreadLockData = m_lockData.localData();

    if (pThreadLockData == NULL)
    {
        pThreadLockData = new ThreadLockData;
        m_lockData.setLocalData(pThreadLockData);
    }

    return pThreadLockData;
}


} // namespace BOI

