/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "GuiRequest.h"
#include "GuiRequestPool.h"


namespace BOI {


GuiRequestPool::GuiRequestPool(int initialSize)
{
    if (initialSize > 0)
    {
        m_pHead = new GuiRequest();
        GuiRequest* pRequest = m_pHead;

        for (int i=1; i < initialSize; i++)
        {
            pRequest->pNext = new GuiRequest();
            pRequest = pRequest->pNext;
        }
    }
    else
    {
        m_pHead = NULL;
    }
}


GuiRequestPool::~GuiRequestPool()
{
    GuiRequest* pRequest;

    while (m_pHead != NULL)
    {
        pRequest = m_pHead;
        m_pHead  = m_pHead->pNext;

        delete pRequest;
    }
}


GuiRequest* GuiRequestPool::Get()
{
    GuiRequest* pRequest;

    if (m_pHead == NULL)
    {
        pRequest = new GuiRequest();
    }
    else
    {
        pRequest = m_pHead;
        m_pHead  = m_pHead->pNext;
    }

    return pRequest;
}


void GuiRequestPool::Put(GuiRequest* pRequest)
{
    pRequest->pNext = m_pHead;
    m_pHead = pRequest;
}


void GuiRequestPool::Put(GuiRequest* pHead, GuiRequest* pTail)
{
    pTail->pNext = m_pHead;
    m_pHead = pHead;
}


} // namespace BOI

