/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ActionEngine.h"
#include "ViewTransformers/ScrollViewTransformer.h"
#include "ViewTransformers/KineticScrollViewTransformer.h"
#include "TransformManager.h"


namespace BOI {


TransformManager::TransformManager(View* pView, ActionEngine* pActionEngine)
    : QObject(),
      m_pView(pView),
      m_timer(),
      m_pHead(NULL),
      m_pTail(NULL),
      m_instanceId(0),
      m_pActionEngine(pActionEngine)
{
}


TransformManager::~TransformManager()
{
    StopTransformers();
}


int TransformManager::AppendTransformer(ViewTransformerId id,
                                        int version,
                                        qreal* pArgs)
{
    ViewTransformer* pTransformer = NULL;

    if (id == BOI_VTID(Scroll))
    {
        pTransformer = new ScrollViewTransformer();
    }
    else if (id == BOI_VTID(KineticScroll))
    {
        pTransformer = new KineticScrollViewTransformer();
    }

    if (pTransformer != NULL)
    {
        pTransformer->m_pNext = NULL;
        pTransformer->m_pView = m_pView;
        pTransformer->m_version = version;
        pTransformer->m_instanceId = m_instanceId;
        pTransformer->m_updateInterval = BOI_VIEWTRANSFORM_UPDATE_INTERVAL;

        /*
         * Make sure that -1 is never used for a valid instance id.
         */
        m_instanceId++;
        if (m_instanceId == -1) m_instanceId = 0;


        if (pTransformer->Initialize(version, pArgs))
        {
            if (m_pHead == NULL) m_pHead = pTransformer;
            else m_pTail->m_pNext = pTransformer;

            m_pTail = pTransformer;


            if (!m_timer.isActive())
            {
                m_timer.start(BOI_VIEWTRANSFORM_UPDATE_INTERVAL, this);
            }

            return pTransformer->m_instanceId;
        }
        else
        {
            delete pTransformer;
        }
    }

    return -1;
}


bool TransformManager::UpdateTransformer(int instanceId, qreal* pArgs)
{
    ViewTransformer* pTransformer = m_pHead;

    while (pTransformer != NULL)
    {
        if (pTransformer->m_instanceId == instanceId)
        {
            pTransformer->Reset();

            if (pTransformer->Initialize(pTransformer->m_version, pArgs))
            {
                return true;
            }
            else
            {
                StopTransformer(instanceId);
                return false;
            }
        }

        pTransformer = pTransformer->m_pNext;
    }

    return false;
}


void TransformManager::StopTransformers()
{
    if (m_timer.isActive())
    {
        m_timer.stop();
    }

    ViewTransformer* pTransformer = m_pHead;
    ViewTransformer* pNext = NULL;

    while (pTransformer != NULL)
    {
        pNext = pTransformer->m_pNext;
        delete pTransformer;
        pTransformer = pNext;
    }

    m_pHead = NULL;
    m_pTail = NULL;
}


void TransformManager::StopTransformer(int instanceId)
{
    if (m_pHead == NULL) return;

    if (m_pHead->m_instanceId == instanceId)
    {
        ViewTransformer* pNext = m_pHead->m_pNext;
        delete m_pHead;
        m_pHead = pNext;
    }
    else
    {
        ViewTransformer* pTransformer = m_pHead->m_pNext;
        ViewTransformer* pPrev = m_pHead;

        while (pTransformer != NULL)
        {
            if (pTransformer->m_instanceId == instanceId)
            {
                pPrev->m_pNext = pTransformer->m_pNext;
                delete pTransformer;
                break;
            }
            else
            {
                pPrev = pTransformer;
                pTransformer = pTransformer->m_pNext;
            }
        }
    }

    if (m_pHead == NULL)
    {
        if (m_timer.isActive())
        {
            m_timer.stop();
        }

        m_pTail = NULL;
    }
}


void TransformManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    ViewTransformer* pHead = NULL;
    ViewTransformer* pTail = NULL;
    ViewTransformer* pNext = NULL;
    ViewTransformer* pTransformer = m_pHead;

    while (pTransformer != NULL)
    {
        pNext = pTransformer->m_pNext;

        if (pTransformer->Run())
        {
            if (pHead == NULL) pHead = pTransformer;
            else pTail->m_pNext = pTransformer;

            pTail = pTransformer;
        }
        else
        {
            delete pTransformer;
        }

        pTransformer = pNext;
    }

    m_pHead = pHead;
    m_pTail = pTail;

    if (m_pTail != NULL)
    {
        m_pTail->m_pNext = NULL;
    }
    else
    {
        /*
         * If there are no more transformers
         * to run then stop the timer.
         */
        m_timer.stop();
    }


    if (m_pActionEngine->HasActiveAction())
    {
        /*
         * Notify the active action that the
         * View has been transformed.
         */
        m_pActionEngine->Execute(BOI_AEOP(NotifyViewTransformed));
    }
}


} // namespace BOI

