/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "Data.h"
#include "DataManager.h"
#include "DRef.h"


namespace BOI {


DRef::DRef(Data* pData)
{
    m_pData = pData;
    m_pData->m_refCount = 1;
}


DRef::DRef(const DRef& dref)
{
    m_pData = dref.m_pData;

    if (m_pData != NULL)
    {
        m_pData->m_refCount.ref();
    }
}


void* DRef::WriteInstance(bool doCopy)
{
    /*
     * If fetchAndAdd returns 1 then it is guaranteed
     * that *this DRef is the only one referencing the
     * Data object. No copy (to create a second reference)
     * could be taking place since the thread is currently
     * in this function and, for a single DRef instance,
     * GetWriteInstance and operator=/CopyConstructor can 
     * not happen at the same time.
     *
     * fetchAndAdd could never return a value less than 1 since
     * at least *this DRef is referencing the Data object.
     *
     * If fetchAndAdd returns a value greater than 1 then
     * there is another DRef out there referencing
     * the same data.
     * There is a chance that the other DRef is in the start
     * of the DRef::Reset() method in which case its reference 
     * will be removed soon. This scenario would remove the need to 
     * perform a copy here but because this might not happen 
     * frequently and using a mutex would still have the same issue,
     * it's probably o.k. to leave the code as is.
     */
    if (m_pData->m_refCount.fetchAndAddAcquire(0) > 1)
    {
        if (doCopy)
        {
            Data* pData = m_pData->m_pDataManager->GetDataInstance(m_pData->Type());
            m_pData->CopyInstance(pData);

            pData->m_refCount = 1;

            if (!m_pData->m_refCount.deref())
            {
                m_pData->m_pDataManager->DeleteDataInstance(m_pData);
            }

            m_pData = pData;
        }
        else
        {
            return NULL;
        }
    }

    return m_pData->Instance();
}


void DRef::Reset()
{
    if (m_pData != NULL)
    {
        if (!m_pData->m_refCount.deref())
        {
            m_pData->m_pDataManager->DeleteDataInstance(m_pData);
        }

        m_pData = NULL;
    }
}


DRef& DRef::operator=(const DRef& dref)
{
    if (this != &dref)
    {
        Reset();

        m_pData = dref.m_pData;

        if (m_pData != NULL)
        {
            m_pData->m_refCount.ref();
        }
    }

    return *this;
}


} // namespace BOI

