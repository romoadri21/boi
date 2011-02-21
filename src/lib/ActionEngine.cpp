/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "Action.h"
#include "ActionArgs.h"
#include "ActionManager.h"
#include "ActionChangedListener.h"
#include "Events/SetActionEvent.h"
#include "Events/UpdateActionEvent.h"
#include "ActionEngine.h"


namespace BOI {


ActionEngine::ActionEngine()
    : m_stack(),
      m_pActionManager(NULL),
      m_pListener(NULL),
      m_pASI(NULL)
{
}


void ActionEngine::SetActionManager(ActionManager* pActionManager)
{
    m_pActionManager = pActionManager;
}


void ActionEngine::SetListener(ActionChangedListener* pListener)
{
    m_pListener = pListener;
}


void ActionEngine::SetASI(ASI* pASI)
{
    m_pASI = pASI;
}


int ActionEngine::ActiveActionType() const
{
    return m_stack.top()->Type();
}


void ActionEngine::Execute(OpCode opCode)
{
    if (opCode == OpCode_NotifyViewTransformed)
    {
        m_stack.top()->HandleViewTransformed(m_pASI);
    }
    else if (opCode == OpCode_Clear)
    {
        ProcessCommand(BOI_AC_CLEAR);
    }
}


void ActionEngine::Execute(OpCode opCode, int value)
{
    if (opCode == OpCode_Set)
    {
        ProcessCommand(BOI_AC_SET(value, NULL));
    }
}


void ActionEngine::Execute(OpCode opCode, bool* pBool)
{
    if (opCode == OpCode_QueryAcceptTouchStream)
    {
        *pBool = m_stack.top()->AcceptTouchStream();
    }
    else if (opCode == OpCode_QueryAcceptKeyStream)
    {
        *pBool = m_stack.top()->AcceptKeyStream();
    }
}


void ActionEngine::Execute(OpCode opCode, KeyEvent* pEvent)
{
    if (opCode == OpCode_SendKeyEvent)
    {
        ActionCommand command = m_stack.top()->HandleKeyEvent(m_pASI, pEvent);
        ProcessCommand(command);
    }
}


void ActionEngine::Execute(OpCode opCode, TouchEvent* pEvent)
{
    if (opCode == OpCode_SendTouchEvent)
    {
        ActionCommand command = m_stack.top()->HandleTouchEvent(m_pASI, pEvent);
        ProcessCommand(command);
    }
}


void ActionEngine::HandleSetActionEvent(SetActionEvent* pEvent)
{
    ProcessCommand(BOI_AC_SET(pEvent->action, pEvent->pArgs));
}


void ActionEngine::HandleUpdateActionEvent(UpdateActionEvent* pEvent)
{
    const ActionArgs* pArgs = pEvent->pArgs;

    if (!m_stack.isEmpty())
    {
        Action* pAction = m_stack.top();

        if (pEvent->action == pAction->Type())
        {
            ActionCommand command = pAction->Update(m_pASI, pArgs);
            ProcessCommand(command);
        }
    }

    if ((pArgs != NULL) && (pArgs->AutoDelete()))
    {
        delete pArgs;
    }
}


void ActionEngine::ProcessCommand(ActionCommand command)
{
    Action* pAction;
    bool advance = true;
    bool notifyListener = (command.Type() != ActionCommand::Type_Continue);

    do
    {
        int type   = command.Type();
        int action = command.Action();
        const ActionArgs* pArgs = command.Args();

        if (type == ActionCommand::Type_Continue)
        {
            advance = false;
        }
        else if (type == ActionCommand::Type_Stop)
        {
            Pop();
            advance = false;
        }
        else if (type == ActionCommand::Type_Replace)
        {
            pAction = m_pActionManager->GetAction(action);

            command = (pAction == NULL) ?
                      (BOI_AC_CLEAR) :
                      (Replace(pAction, pArgs));
        }
        else if (type == ActionCommand::Type_Set)
        {
            pAction = m_pActionManager->GetAction(action);

            command = (pAction == NULL) ?
                      (BOI_AC_CLEAR) :
                      (Push(pAction, pArgs));
        }
        else if (type == ActionCommand::Type_Clear)
        {
            Clear();
            advance = false;
        }
        else if (type == ActionCommand::Type_ClearAndSet)
        {
            Clear();

            pAction = m_pActionManager->GetAction(action);

            command = (pAction == NULL) ?
                      (BOI_AC_CONTINUE) :
                      (Push(pAction, pArgs));
        }

        if ((pArgs != NULL) && (pArgs->AutoDelete()))
        {
            delete pArgs;
        }

    } while (advance);


    if (notifyListener)
    {
        m_pListener->HandleActionChanged();
    }
}


void ActionEngine::Clear()
{
    while (!m_stack.isEmpty())
    {
        Action* pAction = m_stack.pop();
        pAction->Stop(m_pASI);
    }
}


ActionCommand ActionEngine::Push(Action* pNewAction, const ActionArgs* pArgs)
{
    if (!m_stack.isEmpty())
    {
        Action* pAction = m_stack.top();
        if (!pAction->Suspend(m_pASI))
        {
            pAction->Stop(m_pASI);
            m_stack.pop();
        }
    }

    m_stack.push(pNewAction);
    return pNewAction->Start(m_pASI, pArgs);
}


void ActionEngine::Pop()
{
    if (!m_stack.isEmpty())
    {
        Action* pAction = m_stack.pop();
        pAction->Stop(m_pASI);

        while (!m_stack.isEmpty())
        {
            pAction = m_stack.top();

            if (!pAction->Resume(m_pASI))
            {
                pAction->Stop(m_pASI);
                m_stack.pop();
            }
            else
            {
                break;
            }
        }
    }
}


ActionCommand ActionEngine::Replace(Action* pNewAction, const ActionArgs* pArgs)
{
    if (!m_stack.isEmpty())
    {
        Action* pAction = m_stack.pop();
        pAction->Stop(m_pASI);
    }

    m_stack.push(pNewAction);
    return pNewAction->Start(m_pASI, pArgs);
}


} // namespace BOI

