/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TASKPOOL_H
#define __BOI_TASKPOOL_H


#include <QAtomicInt>


namespace BOI {


class Task;


class TaskPool
{
    public:
        TaskPool(int initialSize=37);
        ~TaskPool();

        Task* Get();
        bool Put(Task* pTask);
        bool Put(Task* pHead, Task* pTail);

    private:
        Task* m_pHead;
        QAtomicInt m_refCount;
};


} // namespace BOI


#endif //__BOI_TASKPOOL_H

