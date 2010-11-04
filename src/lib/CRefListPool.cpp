/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "CRefListData.h"
#include "CRefListPool.h"


namespace BOI {


CRefListPool::CRefListPool(int initialSize)
    : m_mutex()
{
    if (initialSize > 0)
    {
        m_pHead = new CRefListData;

        CRefListData* pData = m_pHead;
        pData->crefs = pData->defaultList;
        pData->count = 0;
        pData->capacity = BOI_INITIAL_CREFLIST_CAPACITY;
        pData->refCount = 0;
        pData->pNext = NULL;
        pData->pPool = this;

        for (int i=1; i < initialSize; i++)
        {
            pData->pNext = new CRefListData;

            pData = pData->pNext;
            pData->crefs = pData->defaultList;
            pData->count = 0;
            pData->capacity = BOI_INITIAL_CREFLIST_CAPACITY;
            pData->refCount = 0;
            pData->pNext = NULL;
            pData->pPool = this;
        }
    }
    else
    {
        m_pHead = NULL;
    }
}


CRefListPool::~CRefListPool()
{
    CRefListData* pData;

    while (m_pHead != NULL)
    {
        pData = m_pHead;
        m_pHead = m_pHead->pNext;

        if (pData->crefs != pData->defaultList)
        {
            delete[] pData->crefs;
        }

        delete pData;
    }
}


CRefList CRefListPool::GetList()
{
    CRefList list;
    list.m_pData = GetData();
    list.m_pData->refCount = 1;

    return list;
}


CRefListData* CRefListPool::GetData()
{
    CRefListData* pData = NULL;

    m_mutex.lock();

    if (m_pHead != NULL)
    {
        pData = m_pHead;
        m_pHead = m_pHead->pNext;
        m_mutex.unlock();
    }
    else
    {
        m_mutex.unlock();

        pData = new CRefListData;
        pData->crefs = pData->defaultList;
        pData->count = 0;
        pData->capacity = BOI_INITIAL_CREFLIST_CAPACITY;
        pData->refCount = 0;
        pData->pNext = NULL;
        pData->pPool = this;
    }

    return pData;
}


void CRefListPool::Put(CRefListData* pData)
{
    m_mutex.lock();
    pData->pNext = m_pHead;
    m_pHead = pData;
    m_mutex.unlock();
}


} // namespace BOI

