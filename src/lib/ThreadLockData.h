/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_THREADLOCKDATA_H
#define __BOI_THREADLOCKDATA_H


#include <QWaitCondition>


namespace BOI {


class ThreadLockData
{
    public:
        ThreadLockData* pNext;
        QWaitCondition  waitCondition;
};


} // namespace BOI


#endif //__BOI_THREADLOCKDATA_H

