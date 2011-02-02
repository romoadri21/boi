/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MOVEACTION_H
#define __BOI_MOVEACTION_H


#include "CRef.h"
#include "Action.h"


namespace BOI {


class MoveAction
    : public Action
{
    public:
        MoveAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

        void Destroy();

    private:
        typedef struct
        {
            CRef cref;
            qreal xDelta;
            qreal yDelta;
        } MoveData;

    private:
        void Reserve(int size);
        void ResetData();

    private:
        int m_numTouchStreams;

        MoveData* m_pMoveData;
        int m_capacity;
        int m_count;
};


} // namespace BOI


#endif //__BOI_MOVEACTION_H

