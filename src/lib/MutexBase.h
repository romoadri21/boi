/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MUTEXBASE_H
#define __BOI_MUTEXBASE_H


#include <QMutex>
#include <QThreadStorage>


#define BOI_MUTEXBASE_MAX_QMUTEXES 0x10


namespace BOI {


class Mutex;
class ThreadLockData;


class MutexBase
{
    public:
        MutexBase();

        void Attach(Mutex* pMutex);

        ThreadLockData* GetData();

    private:
        int m_mutexId;
        QMutex m_mutexes[BOI_MUTEXBASE_MAX_QMUTEXES];
        QThreadStorage<ThreadLockData*> m_lockData;
};


} // namespace BOI


#endif //__BOI_MUTEXBASE_H

