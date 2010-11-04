/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TASKPROCESSOR_H
#define __BOI_TASKPROCESSOR_H


#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include "CRef.h"
#include "DRef.h"
#include "StateId.h"
#include "TaskPool.h"
#include "Interface.h"


#define BOI_TASK_PROCESSOR_MAX_COMPLETED_TASKS 10


namespace BOI {


class Task;
class KeyEvent;
class MutexBase;
class Component;
class TouchEvent;
class ComponentData;


class TaskProcessor
{
    public:
        TaskProcessor();

        bool Initialize(int numThreads);
        void Destroy();

        void Start();

        void ActivateComponent(ComponentData* pComponentData);

        void PostKeyEventTask(CRef cref, KeyEvent* pKeyEvent);
        void PostTouchEventTask(CRef cref, TouchEvent* pTouchEvent);
        void PostStateChangedTask(CRef cref, StateId stateId, DRef& dref);
        void PostEmitterConnectedTask(CRef cref, int emitter, int componentId);
        void PostEmitTask(int source, CRef cref, ReceiverFunc func, DRef& dref, bool release);

        void FreeTasks(ComponentData* pComponentData);

    protected:
        class TaskThread : public QThread
        {
            public:
                CRef cref;
                TaskThread* pNext;
                TaskProcessor* pTaskProcessor;

            public:
                void FreeTaskInstance(Task* pTask);

            protected:
                virtual void run();

            private:
                int m_numCompletedTasks;
                Task* m_pCompletedTasksHead;
                Task* m_pCompletedTasksTail;
        };

    protected:
        void Enqueue(Component* pComponent, Task* pTask);
        void ProcessTask(TaskThread* pTaskThread);
        void ProcessTasks(TaskThread* pTaskThread);

    private:
        TaskThread* m_pTaskThreadsHead;

        bool m_processTasks;

        int m_numTasks;
        QMutex m_taskQueueLock;
        QQueue<CRef> m_taskQueue;
        QWaitCondition m_waitCondition;

        TaskPool m_taskPool;
};


} // namespace BOI


#endif //__BOI_TASKPROCESSOR_H

