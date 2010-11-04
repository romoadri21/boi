/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "Task.h"
#include "TaskPool.h"


namespace BOI {


TaskPool::TaskPool(int initialSize)
    : m_pHead(NULL),
      m_refCount(0)
{
    Task* pTask;

    for (int i=0; i < initialSize; i++)
    {
        pTask = new Task();
        pTask->pNext = m_pHead;
        m_pHead = pTask;
    }
}


TaskPool::~TaskPool()
{
    Task* pTask;

    while (m_pHead != NULL)
    {
        pTask = m_pHead;
        m_pHead = m_pHead->pNext;

        delete pTask;
    }
}


Task* TaskPool::Get()
{
    Task* pTask;

    if (m_refCount.testAndSetOrdered(0,1))
    {
        if (m_pHead != NULL)
        {
            pTask = m_pHead;
            m_pHead = m_pHead->pNext;

            m_refCount.deref();
        }
        else
        {
            m_refCount.deref();
            pTask = new Task();
        }
    }
    else
    {
        pTask = new Task();
    }

    return pTask;
}


bool TaskPool::Put(Task* pTask)
{
    if (m_refCount.testAndSetOrdered(0,1))
    {
        pTask->pNext = m_pHead;
        m_pHead = pTask;

        m_refCount.deref();
        return true;
    }

    return false;
}


bool TaskPool::Put(Task* pHead, Task* pTail)
{
    if (m_refCount.testAndSetOrdered(0,1))
    {
        pTail->pNext = m_pHead;
        m_pHead = pHead;

        m_refCount.deref();
        return true;
    }

    return false;
}


} // namespace BOI

