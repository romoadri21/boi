/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QEvent>
#include "Events/SetActionEvent.h"
#include "Events/VirtualKeyEvent.h"
#include "Events/GuiRequestEvent.h"
#include "Events/UpdateActionEvent.h"
#include "Events/InputModeChangeEvent.h"
#include "Factories/CustomEventsFactory.h"


namespace BOI {


CustomEventsFactory::CustomEventsFactory()
{
    m_setActionEventType = QEvent::registerEventType();
    m_virtualKeyEventType = QEvent::registerEventType();
    m_guiRequestEventType = QEvent::registerEventType();
    m_updateActionEventType = QEvent::registerEventType();
    m_inputModeChangeEventType = QEvent::registerEventType();
}


SetActionEvent* CustomEventsFactory::NewSetActionEvent()
{
    return new SetActionEvent(m_setActionEventType);
}


VirtualKeyEvent* CustomEventsFactory::NewVirtualKeyEvent()
{
    return new VirtualKeyEvent(m_virtualKeyEventType);
}


GuiRequestEvent* CustomEventsFactory::NewGuiRequestEvent()
{
    return new GuiRequestEvent(m_guiRequestEventType);
}


UpdateActionEvent* CustomEventsFactory::NewUpdateActionEvent()
{
    return new UpdateActionEvent(m_updateActionEventType);
}


InputModeChangeEvent* CustomEventsFactory::NewInputModeChangeEvent()
{
    return new InputModeChangeEvent(m_inputModeChangeEventType);
}


} // namespace BOI

