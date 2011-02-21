/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ACTIONENGINE_H
#define __BOI_ACTIONENGINE_H


#include <QStack>
#include "ActionCommand.h"
#include "Events/SetActionEventListener.h"
#include "Events/UpdateActionEventListener.h"


#define BOI_AEOP(opcode) (BOI::ActionEngine::OpCode_##opcode)


namespace BOI {


class ASI;
class Action;
class KeyEvent;
class TouchEvent;
class ActionManager;
class ActionChangedListener;


class ActionEngine
    : public SetActionEventListener,
      public UpdateActionEventListener
{
    public:
        enum OpCode
        {
            OpCode_Set = 0,
            OpCode_Clear,
            OpCode_SendKeyEvent,
            OpCode_SendTouchEvent,
            OpCode_NotifyViewTransformed,
            OpCode_QueryAcceptTouchStream,
            OpCode_QueryAcceptKeyStream
        };

    public:
        ActionEngine();

        void SetActionManager(ActionManager* pActionManager);
        void SetListener(ActionChangedListener* pListener);
        void SetASI(ASI* pASI);

        bool HasActiveAction() const;
        int ActiveActionType() const;

        void Execute(OpCode opCode);
        void Execute(OpCode opCode, int value);
        void Execute(OpCode opCode, bool* pBool);
        void Execute(OpCode opCode, KeyEvent* pEvent);
        void Execute(OpCode opCode, TouchEvent* pEvent);

        void HandleSetActionEvent(SetActionEvent* pEvent);
        void HandleUpdateActionEvent(UpdateActionEvent* pEvent);

    protected:
        void ProcessCommand(ActionCommand command);

        void Clear();

        ActionCommand Push(Action* pAction, const ActionArgs* pArgs);
        ActionCommand Replace(Action* pAction, const ActionArgs* pArgs);

    private:
        QStack<Action*> m_stack;
        ActionManager*  m_pActionManager;

        ActionChangedListener* m_pListener;

        ASI* m_pASI;
};


inline bool ActionEngine::HasActiveAction() const
{
    return !m_stack.isEmpty();
}


} // namespace BOI


#endif //__BOI_ACTIONENGINE_H

