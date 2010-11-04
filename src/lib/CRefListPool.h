/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CREFLISTPOOL_H
#define __BOI_CREFLISTPOOL_H


#include <QMutex>
#include "CRefList.h"


namespace BOI {


class CRefListData;


class CRefListPool
{
    public:
        CRefListPool(int initialSize=7);
        ~CRefListPool();

        CRefList GetList();
        CRefListData* GetData();

        void Put(CRefListData* pData);

    private:
        CRefListData* m_pHead;
        QMutex m_mutex;
};


} // namespace BOI


#endif //__BOI_CREFLISTPOOL_H

