/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "CRefListData.h"
#include "CRefListPool.h"
#include "CRefList.h"


namespace BOI {


CRefList::CRefList()
    : m_pData(NULL)
{
}


CRefList::~CRefList()
{
    Reset();
}


CRefList::CRefList(const CRefList& crefList)
{
    m_pData = crefList.m_pData;

    if (m_pData != NULL)
    {
        m_pData->refCount.ref();
    }
}


int CRefList::Count()
{
    return (m_pData != NULL) ? m_pData->count : 0;
}


CRef CRefList::Value(int i)
{
    Q_ASSERT(i < m_pData->count);
    return m_pData->crefs[i];
}


void CRefList::Append(CRef& cref)
{
    int count = 0;

    if (m_pData == NULL)
    {
        m_pData = new CRefListData;
        m_pData->crefs = m_pData->defaultList;
        m_pData->capacity = BOI_INITIAL_CREFLIST_CAPACITY;
        m_pData->refCount = 1;
        m_pData->pPool = NULL;
        m_pData->count = 0;
    }
    else
    {
        if (m_pData->refCount.fetchAndAddRelaxed(0) > 1)
        {
            DeepCopy();
        }

        count = m_pData->count;

        if (count == m_pData->capacity)
        {
            int newCapacity = m_pData->capacity * 2;
            CRef* crefs = new CRef[newCapacity];

            for (int i=0; i < count; i++)
            {
                crefs[i] = m_pData->crefs[i];
                m_pData->crefs[i].Reset();
            }

            if (m_pData->crefs != m_pData->defaultList)
            {
                delete[] m_pData->crefs;
            }

            m_pData->crefs = crefs;
            m_pData->capacity = newCapacity;
        }
    }

    m_pData->crefs[count] = cref;
    m_pData->count++;
}


void CRefList::DeepCopy()
{
    CRefListData* pData;

    if (m_pData->pPool != NULL)
    {
        pData = m_pData->pPool->GetData();

        if (pData->capacity < m_pData->capacity)
        {
            if (pData->crefs != pData->defaultList)
            {
                delete[] pData->crefs;
            }

            pData->capacity = m_pData->capacity;
            pData->crefs = new CRef[pData->capacity];
        }
    }
    else
    {
        pData = new CRefListData;
        pData->capacity = m_pData->capacity;
        pData->pPool = NULL;

        if (pData->capacity == BOI_INITIAL_CREFLIST_CAPACITY)
        {
            pData->crefs = pData->defaultList;
        }
        else
        {
            pData->crefs = new CRef[pData->capacity];
        }
    }

    pData->count = m_pData->count;
    pData->refCount = 1;

    int count = pData->count;
    for (int i=0; i < count; i++)
    {
        pData->crefs[i] = m_pData->crefs[i];
    }

    Reset();

    m_pData = pData;
}


void CRefList::Reset()
{
    if (m_pData != NULL)
    {
        if (!m_pData->refCount.deref())
        {
            if (m_pData->pPool != NULL)
            {
                int count = m_pData->count;
                for (int i=0; i < count; i++)
                {
                    m_pData->crefs[i].Reset();
                }

                m_pData->count = 0;
                m_pData->pPool->Put(m_pData);
            }
            else
            {
                if (m_pData->crefs != m_pData->defaultList)
                {
                    delete[] m_pData->crefs;
                }

                delete m_pData;
            }
        }

        m_pData = NULL;
    }
}


CRefList& CRefList::operator=(const CRefList& crefList)
{
    if (this != &crefList)
    {
        Reset();

        m_pData = crefList.m_pData;

        if (m_pData != NULL)
        {
            m_pData->refCount.ref();
        }
    }

    return *this;
}


} // namespace BOI

