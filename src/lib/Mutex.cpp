/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QMutex>
#include <QWaitCondition>
#include "ThreadLockData.h"
#include "MutexBase.h"
#include "Mutex.h"


namespace BOI {


Mutex::Mutex()
    : m_numAccessors(0),
      m_doWait(true),
      m_pHead(NULL),
      m_pTail(NULL),
      m_pMutex(NULL),
      m_pBase(NULL)
{
}


void Mutex::Lock()
{
    if (m_numAccessors.fetchAndAddAcquire(1) != 0)
    {
        m_pMutex->lock();

        if (m_doWait || (m_pHead != NULL))
        {
            ThreadLockData* pThreadLockData = m_pBase->GetData();
            pThreadLockData->pNext = NULL;

            /*
             * Enqueue the thread.
             */
            if (m_pHead == NULL)
            {
                m_pHead = pThreadLockData;
            }
            else
            {
                m_pTail->pNext = pThreadLockData;
            }
            m_pTail = pThreadLockData;

            pThreadLockData->waitCondition.wait(m_pMutex);

            /*
             * Dequeue thread.
             */
            m_pHead = m_pHead->pNext;
        }

        m_doWait = true;

        m_pMutex->unlock();
    }
}


void Mutex::Unlock()
{
    if (m_numAccessors.fetchAndAddRelease(-1) != 1)
    {
        m_pMutex->lock();

        m_doWait = false;

        if (m_pHead != NULL)
        {
            m_pHead->waitCondition.wakeOne();
        }

        m_pMutex->unlock();
    }
}


} // namespace BOI

