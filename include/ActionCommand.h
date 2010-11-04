/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ACTIONCOMMAND_H
#define __BOI_ACTIONCOMMAND_H


#include "StandardActions.h"


#define BOI_AC_CONTINUE                     (BOI::ActionCommand())
#define BOI_AC_STOP                         (BOI::ActionCommand(BOI::ActionCommand::Type_Stop))
#define BOI_AC_CLEAR                        (BOI::ActionCommand(BOI::ActionCommand::Type_Clear))
#define BOI_AC_SET(action, pArgs)           (BOI::ActionCommand(BOI::ActionCommand::Type_Set, action, pArgs))
#define BOI_AC_REPLACE(action, pArgs)       (BOI::ActionCommand(BOI::ActionCommand::Type_Replace, action, pArgs))
#define BOI_AC_CLEAR_AND_SET(action, pArgs) (BOI::ActionCommand(BOI::ActionCommand::Type_ClearAndSet, action, pArgs))


namespace BOI {


class ActionArgs;


/*
 * The ActionCommand types:
 *
 * SET = Tries to suspend the current action and sets the active action to the
 *      specified action. Calls Action::Start(...) on the new action and passes
 *      control to it.
 * STOP = Stops the current action and calls Action::Stop(...) on it. This
 *      notifies the action engine that the current action has completed and
 *      can be removed from further processing. If there are any previously
 *      suspended actions, the first one that can be resumed (processed in the
 *      opposite order they were suspended) becomes the new active action.
 * CLEAR = Stops the active action and all suspended actions. Action::Stop(...)
 *      is called for each one.
 * REPLACE = Replace the current action with the specified action.
 *      Action::Stop(...) is called on the current action, Action::Start(...)
 *      is called on the new action and control passes on to the new action.
 * CONTINUE = Do nothing and continue on with normal processing.
 * CLEAR_AND_SET = Performs a CLEAR and then sets the active action to the
 *      specified action. Action::Start(...) is called on the new action.
 */
class ActionCommand
{
    public:
        enum
        {
            Type_Set = 0,
            Type_Stop,
            Type_Clear,
            Type_Replace,
            Type_Continue,
            Type_ClearAndSet
        };

    public:
        ActionCommand(int type = Type_Continue,
                      int action = BOI_STD_A(Null),
                      const ActionArgs* pArgs = NULL);

        int Type() const;
        int Action() const;

        const ActionArgs* Args() const;

    private:
        int m_type;
        int m_action;
        const ActionArgs* m_pArgs;
};


inline ActionCommand::ActionCommand(int type,
                                    int action,
                                    const ActionArgs* pArgs)
    : m_type(type),
      m_action(action),
      m_pArgs(pArgs)
{
}


inline int ActionCommand::Type() const
{
    return m_type;
}


inline int ActionCommand::Action() const
{
    return m_action;
}


inline const ActionArgs* ActionCommand::Args() const
{
    return m_pArgs;
}


} // namespace BOI


#endif //__BOI_ACTIONCOMMAND_H

