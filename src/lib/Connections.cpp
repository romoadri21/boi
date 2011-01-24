/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ISI.h"
#include "Component.h"
#include "Connections.h"


namespace BOI {


Connections::Connections()
    : m_numCallers(0),
      m_numEmitters(0),
      m_callerTargetsLock(),
      m_callerTargets(NULL),
      m_emitterRecipientsLock(),
      m_emitterRecipients(NULL),
      m_componentId(-1),
      m_pISI(NULL)
{
}


void Connections::Initialize(ISI* pISI, Interface* pInterface, int componentId)
{
    m_componentId = componentId;

    m_pISI = pISI;
    m_pISI->RegisterMutex(&m_callerTargetsLock);
    m_pISI->RegisterMutex(&m_emitterRecipientsLock);


    m_numEmitters = pInterface->NumEmitters();

    if (m_numEmitters > 0)
    {
        m_emitterRecipients = new EmitterRecipientsHead[m_numEmitters];

        for (int i=0; i < m_numEmitters; i++)
        {
            m_emitterRecipients[i].numNewRecipients = 0;
            m_emitterRecipients[i].numEmitterRecipients = 0;
            m_emitterRecipients[i].pHead = NULL;
        }
    }


    m_numCallers = pInterface->NumCallers();

    if (m_numCallers > 0)
    {
        m_callerTargets = new CallerTarget[m_numCallers];

        for (int i=0; i < m_numCallers; i++)
        {
            m_callerTargets[i].numFuncs = 0;
            m_callerTargets[i].pFuncs = NULL;
            m_callerTargets[i].pComponent = NULL;
            m_callerTargets[i].inTransaction = false;
        }
    }
}


void Connections::Destroy()
{
    for (int i=0; i < m_numEmitters; i++)
    {
        EmitterRecipient* pEmitterRecipient = m_emitterRecipients[i].pHead;
        EmitterRecipient* pTempRecipient;

        while (pEmitterRecipient != NULL)
        {
            pTempRecipient = pEmitterRecipient;
            pEmitterRecipient = pEmitterRecipient->pNext;
            delete pTempRecipient;
        }
    }

    delete[] m_emitterRecipients;


    for (int i=0; i < m_numCallers; i++)
    {
        /*
         * If a transaction has not ended by the time that Destroy()
         * is called then release the cref here since it has not been
         * and will not be released in EndTransaction(...).
         */
        if (m_callerTargets[i].inTransaction)
        {
            m_callerTargets[i].pComponent->TransactionEnded(m_callerTargets[i].id);
            m_callerTargets[i].cref.ReleaseInstance();
            m_callerTargets[i].inTransaction = false;
        }
    }

    delete[] m_callerTargets;
}


bool Connections::AddEmitterRecipient(int emitter, CRef cref, ReceiverFunc func)
{
    bool alreadyConnected = false;

    m_emitterRecipientsLock.Lock();

    EmitterRecipient* pEmitterRecipient = m_emitterRecipients[emitter].pHead;

    while (pEmitterRecipient != NULL)
    {
        if (pEmitterRecipient->cref == cref)
        {
            alreadyConnected = true;
            break;
        }

        pEmitterRecipient = pEmitterRecipient->pNext;
    }

    if (!alreadyConnected)
    {
        pEmitterRecipient = new EmitterRecipient;
        pEmitterRecipient->cref = cref;
        pEmitterRecipient->func = func;
        pEmitterRecipient->isNew = true;
        pEmitterRecipient->pNext = m_emitterRecipients[emitter].pHead;

        m_emitterRecipients[emitter].numNewRecipients++;
        m_emitterRecipients[emitter].numEmitterRecipients++;
        m_emitterRecipients[emitter].pHead = pEmitterRecipient;
    }

    m_emitterRecipientsLock.Unlock();

    return !alreadyConnected;
}


void Connections::RemoveEmitterRecipient(int emitter, CRef cref)
{
    m_emitterRecipientsLock.Lock();

    EmitterRecipient* pEmitterRecipient = m_emitterRecipients[emitter].pHead;

    if (pEmitterRecipient != NULL)
    {
        if (pEmitterRecipient->cref == cref)
        {
            if (pEmitterRecipient->isNew)
            {
                m_emitterRecipients[emitter].numNewRecipients--;
            }

            m_emitterRecipients[emitter].numEmitterRecipients--;
            m_emitterRecipients[emitter].pHead = pEmitterRecipient->pNext;
            delete pEmitterRecipient;
        }
        else
        {
            while (pEmitterRecipient->pNext != NULL)
            {
                if (pEmitterRecipient->pNext->cref == cref)
                {
                    if (pEmitterRecipient->pNext->isNew)
                    {
                        m_emitterRecipients[emitter].numNewRecipients--;
                    }

                    m_emitterRecipients[emitter].numEmitterRecipients--;

                    EmitterRecipient* pTempRecipient = pEmitterRecipient->pNext;
                    pEmitterRecipient->pNext = pTempRecipient->pNext;
                    delete pTempRecipient;
                    break;
                }

                pEmitterRecipient = pEmitterRecipient->pNext;
            }
        }
    }

    m_emitterRecipientsLock.Unlock();
}


bool Connections::EmitterHasNew(int emitter)
{
    m_emitterRecipientsLock.Lock();
    bool hasNew = (m_emitterRecipients[emitter].numNewRecipients > 0);
    m_emitterRecipientsLock.Unlock();

    return hasNew;
}


bool Connections::EmitterConnected(int emitter)
{
    return (m_emitterRecipients[emitter].numEmitterRecipients > 0);
}


void Connections::Emit(int emitter, DRef& data, bool release, bool newOnly)
{
    EmitterRecipient* pNextRecipient;
    EmitterRecipient* pValidRecipientsHead = NULL;
    EmitterRecipient* pInvalidRecipientsHead = NULL;

    m_emitterRecipientsLock.Lock();

    bool doRelease = release && (m_emitterRecipients[emitter].numEmitterRecipients == 1);

    EmitterRecipient* pEmitterRecipient = m_emitterRecipients[emitter].pHead;

    while (pEmitterRecipient != NULL)
    {
        pNextRecipient = pEmitterRecipient->pNext;

        if (!pEmitterRecipient->cref.IsDestroyed())
        {
            if (!newOnly || pEmitterRecipient->isNew)
            {
                m_pISI->PostEmitTask(m_componentId,
                                     pEmitterRecipient->cref,
                                     pEmitterRecipient->func,
                                     data,
                                     doRelease);

                pEmitterRecipient->isNew = false;
            }

            pEmitterRecipient->pNext = pValidRecipientsHead;
            pValidRecipientsHead = pEmitterRecipient;
        }
        else
        {
            pEmitterRecipient->pNext = pInvalidRecipientsHead;
            pInvalidRecipientsHead = pEmitterRecipient;
        }

        pEmitterRecipient = pNextRecipient;
    }

    m_emitterRecipients[emitter].numNewRecipients = 0;
    m_emitterRecipients[emitter].pHead = pValidRecipientsHead;

    m_emitterRecipientsLock.Unlock();

    if (release && !doRelease)
    {
        /*
         * If the caller requested a release of the DRef and it
         * couldn't be released because there is more than one
         * recipient then release it here.
         */
        data.Reset();
    }

    while (pInvalidRecipientsHead != NULL)
    {
        pNextRecipient = pInvalidRecipientsHead->pNext;
        delete pInvalidRecipientsHead;
        pInvalidRecipientsHead = pNextRecipient;
    }
}


bool Connections::EmitTo(int emitter, DRef& data, int componentId, bool release)
{
    bool emitted = false;

    m_emitterRecipientsLock.Lock();

    EmitterRecipient* pEmitterRecipient = m_emitterRecipients[emitter].pHead;

    while (pEmitterRecipient != NULL)
    {
        if (pEmitterRecipient->cref.Id() == componentId)
        {
            m_pISI->PostEmitTask(m_componentId,
                                 pEmitterRecipient->cref,
                                 pEmitterRecipient->func,
                                 data,
                                 release);

            if (pEmitterRecipient->isNew)
            {
                pEmitterRecipient->isNew = false;

                m_emitterRecipients[emitter].numNewRecipients--;
            }

            emitted = true;
            break;
        }

        pEmitterRecipient = pEmitterRecipient->pNext;
    }

    m_emitterRecipientsLock.Unlock();

    return emitted;
}


bool Connections::SetCallerTarget(int caller,
                                  CRef cref,
                                  int funcSet,
                                  int numFuncs,
                                  const FuncSetFunc* pFuncs)
{
    bool targetSet = false;

    m_callerTargetsLock.Lock();

    CallerTarget* pTarget = &m_callerTargets[caller];
    if (!pTarget->inTransaction)
    {
        pTarget->cref = cref;
        pTarget->pFuncs = pFuncs;
        pTarget->funcSet = funcSet;
        pTarget->numFuncs = numFuncs;

        targetSet = true;
    }

    m_callerTargetsLock.Unlock();

    return targetSet;
}


bool Connections::ClearCallerTarget(int caller)
{
    bool targetCleared = false;

    m_callerTargetsLock.Lock();

    CallerTarget* pTarget = &m_callerTargets[caller];
    if (!pTarget->inTransaction)
    {
        pTarget->cref.Reset();
        pTarget->numFuncs = 0;
        pTarget->pFuncs = NULL;
        pTarget->pComponent = NULL;

        targetCleared = true;
    }

    m_callerTargetsLock.Unlock();

    return targetCleared;
}


bool Connections::StartTransaction(int caller)
{
    m_callerTargetsLock.Lock();

    CallerTarget* pTarget = &m_callerTargets[caller];
    if ((!pTarget->inTransaction) && (pTarget->cref.IsValid()))
    {
        pTarget->pComponent = pTarget->cref.GetInstance();
        if (pTarget->pComponent != NULL)
        {
            pTarget->id = pTarget->pComponent->AcceptTransaction(pTarget->funcSet);
            if (pTarget->id != -1)
            {
                pTarget->inTransaction = true;
            }
            else
            {
                /*
                 * The Component did not accept the request to start
                 * a transaction. Release the retrieved instance.
                 */
                pTarget->cref.ReleaseInstance();
                pTarget->pComponent = NULL;
            }
        }
        else
        {
            /*
             * Reset the CallerTarget since it can no longer be
             * used as a FuncSet because the target Component
             * has been destroyed.
             */
            pTarget->cref.Reset();
            pTarget->numFuncs = 0;
            pTarget->pFuncs = NULL;
            pTarget->pComponent = NULL;
        }
    }

    m_callerTargetsLock.Unlock();

    return pTarget->inTransaction;
}


void Connections::EndTransaction(int caller)
{
    m_callerTargetsLock.Lock();

    CallerTarget* pTarget = &m_callerTargets[caller];
    if (pTarget->inTransaction)
    {
        pTarget->pComponent->TransactionEnded(pTarget->id);
        pTarget->cref.ReleaseInstance();
        pTarget->inTransaction = false;
        pTarget->pComponent = NULL;
    }

    m_callerTargetsLock.Unlock();
}


DRef Connections::Call(int caller, int func, DRef& dref, bool passThru)
{
    CallerTarget* pTarget = &m_callerTargets[caller];
    if (pTarget->inTransaction)
    {
        if ((func >= 0) && (func < pTarget->numFuncs))
        {
            FuncSetFunc funcSetFunc = pTarget->pFuncs[func];

            if (passThru)
            {
                return funcSetFunc(pTarget->pComponent,
                                   pTarget->id,
                                   dref);
            }
            else
            {
                DRef drefCopy = dref;
                return funcSetFunc(pTarget->pComponent,
                                   pTarget->id,
                                   drefCopy);
            }
        }
    }

    return DRef();
}


} // namespace BOI

