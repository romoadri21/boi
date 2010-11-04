/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MUTEX_H
#define __BOI_MUTEX_H


#include <QAtomicInt>
#include "BOILib.h"


class QMutex;


namespace BOI {


class MutexBase;
class ThreadLockData;


/*
 * Requirements:
 *  1. Efficient when there is only one thread locking the mutex.
 *  2. Efficient resource usage. There can be thousands to millions
 *      of Mutex instances and using an 'OS mutex' per instance, for
 *      example, would be inefficient (and on some systems not possible).
 *  3. Avoid mutex level thread starvation. If a thread is attempting to
 *      acquire the mutex, it should be acquired within a reasonable amount
 *      of time. A thread waiting to acquire the lock should not be preempted
 *      by other threads an indefinite number of times.
 *
 * Mutex does not support recursive locks/unlocks.
 */
class BOI_LIB_EXPORT Mutex
{
    public:
        Mutex();

        void Lock();
        void Unlock();

    private:
        QAtomicInt m_numAccessors;

        bool m_doWait;

        ThreadLockData* m_pHead;
        ThreadLockData* m_pTail;

        QMutex* m_pMutex;
        MutexBase* m_pBase;
        friend class MutexBase;
};


} // namespace BOI


#endif //__BOI_MUTEX_H

