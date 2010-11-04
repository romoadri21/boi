/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CUSTOMEVENTSFACTORY_H
#define __BOI_CUSTOMEVENTSFACTORY_H


namespace BOI {


class SetActionEvent;
class GuiRequestEvent;
class UpdateActionEvent;
class InputModeChangeEvent;


class CustomEventsFactory
{
    public:
        CustomEventsFactory();

        SetActionEvent* NewSetActionEvent();
        GuiRequestEvent* NewGuiRequestEvent();
        UpdateActionEvent* NewUpdateActionEvent();
        InputModeChangeEvent* NewInputModeChangeEvent();

        int SetActionEventType();
        int GuiRequestEventType();
        int UpdateActionEventType();
        int InputModeChangeEventType();

    private:
        int m_setActionEventType;
        int m_guiRequestEventType;
        int m_updateActionEventType;
        int m_inputModeChangeEventType;
};


inline int CustomEventsFactory::SetActionEventType()
{
    return m_setActionEventType;
}


inline int CustomEventsFactory::GuiRequestEventType()
{
    return m_guiRequestEventType;
}


inline int CustomEventsFactory::UpdateActionEventType()
{
    return m_updateActionEventType;
}


inline int CustomEventsFactory::InputModeChangeEventType()
{
    return m_inputModeChangeEventType;
}


} // namespace BOI


#endif //__BOI_CUSTOMEVENTSFACTORY_H

