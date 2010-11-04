/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QEvent>
#include "Factories/CustomEventsFactory.h"
#include "Events/EventDispatcher.h"


namespace BOI {


EventDispatcher::EventDispatcher()
    : QObject(),
      m_pInputModeChangeEventListener(NULL),
      m_pUpdateActionEventListener(NULL),
      m_pGuiRequestEventListener(NULL),
      m_pSetActionEventListener(NULL),
      m_pResizeEventListener(NULL),
      m_pTouchEventListener(NULL),
      m_pCloseEventListener(NULL),
      m_pKeyEventListener(NULL),
      m_pCustomEventsFactory(NULL),
      m_setActionEventType(QEvent::None),
      m_guiRequestEventType(QEvent::None),
      m_updateActionEventType(QEvent::None),
      m_inputModeChangeEventType(QEvent::None)
{
}


void EventDispatcher::SetCustomEventsFactory(CustomEventsFactory* pFactory)
{
    m_pCustomEventsFactory = pFactory;

    m_setActionEventType = pFactory->SetActionEventType();
    m_guiRequestEventType = pFactory->GuiRequestEventType();
    m_updateActionEventType = pFactory->UpdateActionEventType();
    m_inputModeChangeEventType = pFactory->InputModeChangeEventType();
}


bool EventDispatcher::event(QEvent* pEvent)
{
    if (pEvent->type() == m_guiRequestEventType)
    {
        DispatchGuiRequestEvent();
        pEvent->setAccepted(true);
        return true;
    }
    else if (pEvent->type() == m_setActionEventType)
    {
        DispatchSetActionEvent((SetActionEvent*)pEvent);
        pEvent->setAccepted(true);
        return true;
    }
    else if (pEvent->type() == m_updateActionEventType)
    {
        DispatchUpdateActionEvent((UpdateActionEvent*)pEvent);
        pEvent->setAccepted(true);
        return true;
    }
    else if (pEvent->type() == m_inputModeChangeEventType)
    {
        DispatchInputModeChangeEvent((InputModeChangeEvent*)pEvent);
        pEvent->setAccepted(true);
        return true;
    }

    return QObject::event(pEvent);
}


} // namespace BOI

