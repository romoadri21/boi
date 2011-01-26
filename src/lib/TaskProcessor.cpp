/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "Task.h"
#include "MutexBase.h"
#include "Component.h"
#include "ComponentData.h"
#include "TaskProcessor.h"


namespace BOI {


TaskProcessor::TaskProcessor()
    : m_pTaskThreadsHead(NULL),
      m_processTasks(true),
      m_numTasks(0),
      m_taskQueueLock(),
      m_taskQueue(),
      m_waitCondition(),
      m_taskPool()
{
}


bool TaskProcessor::Initialize(int numThreads)
{
    TaskThread* pTaskThread;

    for (int i=0; i < numThreads; i++)
    {
        pTaskThread = new TaskThread();
        pTaskThread->pTaskProcessor = this;
        pTaskThread->pNext = m_pTaskThreadsHead;
        m_pTaskThreadsHead = pTaskThread;
    }

    return true;
}


void TaskProcessor::Destroy()
{
    m_taskQueueLock.lock();
    m_processTasks = false;
    m_taskQueueLock.unlock();

    m_waitCondition.wakeAll();

    /*
     * Wait for all the TaskThreads to exit the run()
     * method and delete them.
     */
    while (m_pTaskThreadsHead != NULL)
    {
        m_pTaskThreadsHead->wait();
        TaskThread* pTaskThread = m_pTaskThreadsHead;
        m_pTaskThreadsHead = m_pTaskThreadsHead->pNext;

        delete pTaskThread;
    }

    /*
     * Delete any remaining crefs from the task queue.
     * Note, no more enqueues should happen while this
     * method is executed.
     */
    while (!m_taskQueue.isEmpty())
    {
        m_taskQueue.dequeue();
    }
}


void TaskProcessor::Start()
{
    TaskThread* pTaskThread = m_pTaskThreadsHead;

    while (pTaskThread != NULL)
    {
        pTaskThread->start();
        pTaskThread = pTaskThread->pNext;
    }
}


void TaskProcessor::ActivateComponent(ComponentData* pComponentData)
{
    /*
     * All new components start with their numActiveTasks at 1
     * even though there are no queued tasks. This prevents
     * a TaskThread from processing any queued tasks until
     * ActivateComponent(...) is called. Note, this method
     * should only be called once on a Component.
     */

    int numTasks = pComponentData->numActiveTasks.fetchAndStoreRelaxed(0);

    /*
     * Subtract one from the numTasks since
     * it started out at 1.
     */
    numTasks--;

    if (numTasks > 0)
    {
        CRef cref = pComponentData->cref;

        m_taskQueueLock.lock();

        m_numTasks += numTasks;

        while (numTasks > 0)
        {
            m_taskQueue.enqueue(cref);
            numTasks--;
        }

        m_taskQueueLock.unlock();
        m_waitCondition.wakeOne();
    }
}


void TaskProcessor::PostTouchEventTask(CRef cref, TouchEvent* pTouchEvent)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        Task* pTask = m_taskPool.Get();
        pTask->type = Task::TaskType_HandleTouchEvent;
        pTask->data.touchEvent = *pTouchEvent;

        Enqueue(pComponent, pTask);

        cref.ReleaseInstance();
    }
}


void TaskProcessor::PostKeyEventTask(CRef cref, KeyEvent* pKeyEvent)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        Task* pTask = m_taskPool.Get();
        pTask->type = Task::TaskType_HandleKeyEvent;
        pTask->data.keyEvent = *pKeyEvent;

        Enqueue(pComponent, pTask);

        cref.ReleaseInstance();
    }
}


void TaskProcessor::PostStateChangedTask(CRef cref, StateId stateId, DRef& dref)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        Task* pTask = m_taskPool.Get();
        pTask->dref = dref;
        pTask->type = Task::TaskType_HandleStateChanged;
        pTask->data.stateId = stateId;

        Enqueue(pComponent, pTask);

        cref.ReleaseInstance();
    }
}


void TaskProcessor::PostEmitterConnectedTask(CRef cref, int emitter, int componentId)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        Task* pTask = m_taskPool.Get();
        pTask->type = Task::TaskType_HandleEmitterConnected;
        pTask->data.emitterConnectedData.emitter = emitter;
        pTask->data.emitterConnectedData.componentId = componentId;

        Enqueue(pComponent, pTask);

        cref.ReleaseInstance();
    }
}


void TaskProcessor::PostEmitterDisconnectedTask(CRef cref, int emitter, int componentId)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        Task* pTask = m_taskPool.Get();
        pTask->type = Task::TaskType_HandleEmitterDisconnected;
        pTask->data.emitterDisconnectedData.emitter = emitter;
        pTask->data.emitterDisconnectedData.componentId = componentId;

        Enqueue(pComponent, pTask);

        cref.ReleaseInstance();
    }
}


void TaskProcessor::PostEmitTask(int source, CRef cref, ReceiverFunc func, DRef& dref, bool release)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        Task* pTask = m_taskPool.Get();
        pTask->dref = dref;
        pTask->type = Task::TaskType_Emit;
        pTask->data.emitData.func = func;
        pTask->data.emitData.source = source;

        if (release) dref.Reset();

        Enqueue(pComponent, pTask);

        cref.ReleaseInstance();
    }
}


void TaskProcessor::FreeTasks(ComponentData* pComponentData)
{
    if (pComponentData->pTasksHead != NULL)
    {
        Task* pHead = pComponentData->pTasksHead;
        Task* pTail = pComponentData->pTasksTail;
        Task* pTask = pHead;

        do
        {
            pTask->dref.Reset();
            pTask = pTask->pNext;

        } while (pTask != NULL);


        while (!m_taskPool.Put(pHead, pTail))
        {
            // TODO: remove this when the task pool has the
            // lock free stack or something like that since
            // it will only slow things down.
            QThread::yieldCurrentThread();
        }

        pComponentData->pTasksHead = NULL;
        pComponentData->pTasksTail = NULL;
    }
}


void TaskProcessor::Enqueue(Component* pComponent, Task* pTask)
{
    pTask->pNext = NULL;

    ComponentData* pComponentData = pComponent->m_pData;
    pComponentData->tasksLock.Lock();

    if (pComponentData->pTasksHead == NULL)
    {
        pComponentData->pTasksHead = pTask;
    }
    else
    {
        pComponentData->pTasksTail->pNext = pTask;
    }

    pComponentData->pTasksTail = pTask;
    pComponentData->tasksLock.Unlock();


    m_taskQueueLock.lock();
    m_taskQueue.enqueue(pComponentData->cref);
    m_numTasks++;
    m_taskQueueLock.unlock();

    m_waitCondition.wakeOne();
}


void TaskProcessor::ProcessTask(TaskThread* pTaskThread)
{
    Component* pComponent = pTaskThread->cref.GetInstance();
    if (pComponent != NULL)
    {
        ComponentData* pComponentData = pComponent->m_pData;

        if (pComponentData->numActiveTasks.fetchAndAddOrdered(1) == 0)
        {
            do
            {
                pComponentData->tasksLock.Lock();
                Task* pTask = pComponentData->pTasksHead;
                pComponentData->pTasksHead = pTask->pNext;
                pComponentData->tasksLock.Unlock();

                if (pTask->type == Task::TaskType_Emit)
                {
                    pTask->data.emitData.func(pComponent,
                                              pTask->dref,
                                              pTask->data.emitData.source);
                    pTask->dref.Reset();
                }
                else if (pTask->type == Task::TaskType_HandleTouchEvent)
                {
                    pComponent->HandleTouchEvent(&pTask->data.touchEvent);
                }
                else if (pTask->type == Task::TaskType_HandleKeyEvent)
                {
                    pComponent->HandleKeyEvent(&pTask->data.keyEvent);
                }
                else if (pTask->type == Task::TaskType_HandleEmitterConnected)
                {
                    pComponent->HandleEmitterConnectedInternal(pTask->data.emitterConnectedData.emitter,
                                                               pTask->data.emitterConnectedData.componentId);
                }
                else if (pTask->type == Task::TaskType_HandleEmitterDisconnected)
                {
                    pComponent->HandleEmitterDisconnectedInternal(pTask->data.emitterDisconnectedData.emitter,
                                                                  pTask->data.emitterDisconnectedData.componentId);
                }
                else if (pTask->type == Task::TaskType_HandleStateChanged)
                {
                    pComponent->HandleStateChanged(pTask->data.stateId, pTask->dref);
                    pTask->dref.Reset();
                }

                pTaskThread->FreeTaskInstance(pTask);

            } while (pComponentData->numActiveTasks.fetchAndAddOrdered(-1) > 1);
        }

        pTaskThread->cref.ReleaseInstance();
    }

    pTaskThread->cref.Reset();
}


void TaskProcessor::ProcessTasks(TaskThread* pTaskThread)
{
    m_taskQueueLock.lock();

    while (m_processTasks)
    {
        if (m_numTasks > 0)
        {
            pTaskThread->cref = m_taskQueue.dequeue();
            m_numTasks--;

            m_taskQueueLock.unlock();

            ProcessTask(pTaskThread);

            m_taskQueueLock.lock();
        }
        else
        {
            m_waitCondition.wait(&m_taskQueueLock);
        }
    }

    m_taskQueueLock.unlock();
}


void TaskProcessor::TaskThread::run()
{
    m_numCompletedTasks = 0;
    m_pCompletedTasksHead = NULL;
    m_pCompletedTasksTail = NULL;

    pTaskProcessor->ProcessTasks(this);

    /*
     * Return any remaining completed tasks to the task pool.
     */
    if (m_pCompletedTasksHead != NULL)
    {
        while (!pTaskProcessor->m_taskPool.Put(m_pCompletedTasksHead,
                                               m_pCompletedTasksTail))
        {
            // TODO: remove this and replace the code when
            // the task queue uses a lock free data structure.
            yieldCurrentThread();
        }
    }
}


void TaskProcessor::TaskThread::FreeTaskInstance(Task* pTask)
{
    if (m_pCompletedTasksHead == NULL)
    {
        m_pCompletedTasksTail = pTask;
    }

    pTask->pNext = m_pCompletedTasksHead;
    m_pCompletedTasksHead = pTask;
    m_numCompletedTasks++;

    /* 
     * Wait for a few tasks to complete before attempting to add
     * the completed tasks back into the pool to help avoid a high
     * contention situation.
     */
    if (m_numCompletedTasks > BOI_TASK_PROCESSOR_MAX_COMPLETED_TASKS)
    {
        bool emptied = pTaskProcessor->m_taskPool.Put(m_pCompletedTasksHead,
                                                      m_pCompletedTasksTail);

        if (emptied)
        {
            m_numCompletedTasks = 0;
            m_pCompletedTasksHead = NULL;
            m_pCompletedTasksTail = NULL;
        }
    }
}


} // namespace BOI

