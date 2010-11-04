/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CREFLIST_H
#define __BOI_CREFLIST_H


#include "CRef.h"
#include "BOILib.h"


namespace BOI {


class CRefListData;


/*
 * As with CRef, a CRefList should never be used through references
 * or pointers. Always use, pass and return them by value.
 *
 * Note, an instance of a CRefList is designed to only be accessed
 * by one thread at a time (aka. it is not thread safe).
 */
class BOI_LIB_EXPORT CRefList
{
    public:
        CRefList();
        CRefList(const CRefList& crefList);
        ~CRefList();

        int  Count();
        CRef Value(int i);
        void Append(CRef& cref);

        CRefList& operator=(const CRefList& crefList);

    private:
        void DeepCopy();
        void Reset();

    private:
        CRefListData* m_pData;
        friend class CRefListPool;
};


} // namespace BOI


#endif //__BOI_CREFLIST_H

