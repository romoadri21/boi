/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_EVENTDISPATCHER_H
#define __BOI_EVENTDISPATCHER_H


#include <QObject>
#include "Events/InputModeChangeEventListener.h"
#include "Events/UpdateActionEventListener.h"
#include "Events/GuiRequestEventListener.h"
#include "Events/SetActionEventListener.h"
#include "Events/ResizeEventListener.h"
#include "Events/TouchEventListener.h"
#include "Events/CloseEventListener.h"
#include "Events/KeyEventListener.h"


namespace BOI {


class InputModeChangeEvent;
class CustomEventsFactory;
class UpdateActionEvent;
class SetActionEvent;
class ResizeEvent;
class TouchEvent;
class KeyEvent;


class EventDispatcher
    : public QObject
{
    Q_OBJECT

    public:
        EventDispatcher();

        void SetCustomEventsFactory(CustomEventsFactory* pFactory);

        void SetInputModeChangeEventListener(InputModeChangeEventListener* pInputModeChangeEventListener);
        void SetUpdateActionEventListener(UpdateActionEventListener* pUpdateActionEventListener);
        void SetGuiRequestEventListener(GuiRequestEventListener* pGuiRequestEventListener);
        void SetSetActionEventListener(SetActionEventListener*   pSetActionEventListener);
        void SetResizeEventListener(ResizeEventListener*         pResizeEventListener);
        void SetTouchEventListener(TouchEventListener*           pTouchEventListener);
        void SetCloseEventListener(CloseEventListener*           pCloseEventListener);
        void SetKeyEventListener(KeyEventListener*               pKeyEventListener);

        void DispatchInputModeChangeEvent(InputModeChangeEvent* pEvent);
        void DispatchUpdateActionEvent(UpdateActionEvent* pEvent);
        void DispatchSetActionEvent(SetActionEvent* pEvent);
        void DispatchResizeEvent(ResizeEvent* pEvent);
        void DispatchTouchEvent(TouchEvent* pEvent);
        void DispatchKeyEvent(KeyEvent* pEvent);
        void DispatchGuiRequestEvent();
        void DispatchCloseEvent();

    protected:
        bool event(QEvent* pEvent);

    private:
        InputModeChangeEventListener* m_pInputModeChangeEventListener;
        UpdateActionEventListener*    m_pUpdateActionEventListener;
        GuiRequestEventListener*      m_pGuiRequestEventListener;
        SetActionEventListener*       m_pSetActionEventListener;
        ResizeEventListener*          m_pResizeEventListener;
        TouchEventListener*           m_pTouchEventListener;
        CloseEventListener*           m_pCloseEventListener;
        KeyEventListener*             m_pKeyEventListener;

        CustomEventsFactory* m_pCustomEventsFactory;
        int m_setActionEventType;
        int m_guiRequestEventType;
        int m_updateActionEventType;
        int m_inputModeChangeEventType;
};


inline void EventDispatcher::SetInputModeChangeEventListener(InputModeChangeEventListener* pInputModeChangeEventListener)
{
    m_pInputModeChangeEventListener = pInputModeChangeEventListener;
}


inline void EventDispatcher::SetUpdateActionEventListener(UpdateActionEventListener* pUpdateActionEventListener)
{
    m_pUpdateActionEventListener = pUpdateActionEventListener;
}


inline void EventDispatcher::SetGuiRequestEventListener(GuiRequestEventListener* pGuiRequestEventListener)
{
    m_pGuiRequestEventListener = pGuiRequestEventListener;
}


inline void EventDispatcher::SetSetActionEventListener(SetActionEventListener* pSetActionEventListener)
{
    m_pSetActionEventListener = pSetActionEventListener;
}


inline void EventDispatcher::SetResizeEventListener(ResizeEventListener* pResizeEventListener)
{
    m_pResizeEventListener = pResizeEventListener;
}


inline void EventDispatcher::SetTouchEventListener(TouchEventListener* pTouchEventListener)
{
    m_pTouchEventListener = pTouchEventListener;
}


inline void EventDispatcher::SetCloseEventListener(CloseEventListener* pCloseEventListener)
{
    m_pCloseEventListener = pCloseEventListener;
}


inline void EventDispatcher::SetKeyEventListener(KeyEventListener* pKeyEventListener)
{
    m_pKeyEventListener = pKeyEventListener;
}


inline void EventDispatcher::DispatchInputModeChangeEvent(InputModeChangeEvent* pEvent)
{
    m_pInputModeChangeEventListener->HandleInputModeChangeEvent(pEvent);
}


inline void EventDispatcher::DispatchUpdateActionEvent(UpdateActionEvent* pEvent)
{
    m_pUpdateActionEventListener->HandleUpdateActionEvent(pEvent);
}


inline void EventDispatcher::DispatchGuiRequestEvent()
{
    m_pGuiRequestEventListener->HandleGuiRequestEvent();
}


inline void EventDispatcher::DispatchSetActionEvent(SetActionEvent* pEvent)
{
    m_pSetActionEventListener->HandleSetActionEvent(pEvent);
}


inline void EventDispatcher::DispatchResizeEvent(ResizeEvent* pEvent)
{
    m_pResizeEventListener->HandleResizeEvent(pEvent);
}


inline void EventDispatcher::DispatchTouchEvent(TouchEvent* pEvent)
{
    m_pTouchEventListener->HandleTouchEvent(pEvent);
}


inline void EventDispatcher::DispatchKeyEvent(KeyEvent* pEvent)
{
    m_pKeyEventListener->HandleKeyEvent(pEvent);
}


inline void EventDispatcher::DispatchCloseEvent()
{
    m_pCloseEventListener->HandleCloseEvent();
}


} // namespace BOI


#endif //__BOI_EVENTDISPATCHER_H

