/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CREFLISTDATA_H
#define __BOI_CREFLISTDATA_H


#include <QAtomicInt>
#include "CRef.h"


#define BOI_INITIAL_CREFLIST_CAPACITY 13


namespace BOI {


class CRefListPool;


class CRefListData
{
    public:
        CRef* crefs;
        CRef  defaultList[BOI_INITIAL_CREFLIST_CAPACITY];

        int count;
        int capacity;

        QAtomicInt refCount;

        CRefListData* pNext;
        CRefListPool* pPool;
};


} // namespace BOI


#endif //__BOI_CREFLISTDATA_H

