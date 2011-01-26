/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TASK_H
#define __BOI_TASK_H


#include "DRef.h"
#include "StateId.h"
#include "Interface.h"
#include "Events/KeyEvent.h"
#include "Events/TouchEvent.h"


namespace BOI {


class Task
{
    public:
        enum
        {
            TaskType_Emit = 0,
            TaskType_HandleKeyEvent,
            TaskType_HandleTouchEvent,
            TaskType_HandleStateChanged,
            TaskType_HandleEmitterConnected,
            TaskType_HandleEmitterDisconnected
        };

    public:
        int  type;
        DRef dref;

        union
        {
            StateId stateId;
            KeyEvent keyEvent;
            TouchEvent touchEvent;

            struct
            {
                int source;
                ReceiverFunc func;
            } emitData;

            struct
            {
                int emitter;
                int componentId;
            } emitterConnectedData;

            struct
            {
                int emitter;
                int componentId;
            } emitterDisconnectedData;
        } data;

        Task* pNext;

    public:
        Task();
};


inline Task::Task()
    : dref()
{
}


} // namespace BOI


#endif //__BOI_TASK_H

