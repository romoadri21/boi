/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "ASI.h"
#include "ISI.h"
#include "CRefList.h"
#include "Component.h"
#include "InputMode.h"
#include "ViewLayerId.h"
#include "ActionEngine.h"
#include "ComponentData.h"
#include "StandardActions.h"
#include "Events/InputModeChangeEvent.h"
#include "InputEventHandler.h"


namespace BOI {


InputEventHandler::InputEventHandler(ActionEngine* pActionEngine)
    : m_pActionEngine(pActionEngine),
      m_pASI(NULL),
      m_pISI(NULL),
      m_pEventStreamsHead(NULL),
      m_availableEventStreamsTop(0),
      m_gestureMode(false),
      m_immediateMode(false),
      m_handleActionChanged(true)
{
    for (int i=0; i < BOI_MAX_EVENTSTREAMS; i++)
    {
        m_availableEventStreams[i] = i;
        m_eventStreams[i].index = i;
    }

    m_pActionEngine->SetListener(this);
}


void InputEventHandler::SetASI(ASI* pASI)
{
    m_pASI = pASI;
}


void InputEventHandler::SetISI(ISI* pISI)
{
    m_pISI = pISI;
}


void InputEventHandler::HandleInputModeChangeEvent(InputModeChangeEvent* pEvent)
{
    if (!m_gestureMode)
    {
        if (pEvent->mode == InputMode_Gesture)
        {
            FlushStreams(true);

            /*
             * When attempting to switch into gesture mode,
             * do not handle action changed notifications
             * since specialized handling of the streams
             * and the mode is required.
             */
            m_handleActionChanged = false;
            m_pActionEngine->Execute(BOI_AEOP(Set), BOI_STD_A(XGesture));
            m_handleActionChanged = true;

            if (m_pActionEngine->HasActiveAction() &&
               (m_pActionEngine->ActiveActionType() == BOI_STD_A(XGesture)))
            {
                /*
                 * If the gesture action is still the active action
                 * after being set then switch into gesture mode.
                 */
                m_gestureMode = true;
            }
        }
    }

    if (pEvent->mode == InputMode_Immediate)
    {
        m_immediateMode = true;
    }
    else if (pEvent->mode == InputMode_Normal)
    {
        m_immediateMode = false;
    }
}


void InputEventHandler::HandleTouchEvent(TouchEvent* pEvent)
{
    EventStream* pEventStream;

    if (pEvent->type == TouchEvent::Type_Move)
    {
        pEventStream = GetStream(EventStream::Type_Touch, pEvent->id);

        if (pEventStream != NULL)
        {
            if (pEventStream->target == EventStream::Target_Component)
            {
                pEventStream->touchEvent = *pEvent;

                QPoint point(pEvent->x, pEvent->y);
                QPointF mappedPoint = m_pASI->MapFromViewToComponent(pEventStream->pComponent, point);
                pEvent->x = mappedPoint.x();
                pEvent->y = mappedPoint.y();

                m_pISI->PostTouchEventTask(pEventStream->cref, pEvent);
            }
            else if (pEventStream->target == EventStream::Target_Action)
            {
                pEventStream->touchEvent = *pEvent;

                m_pActionEngine->Execute(BOI_AEOP(SendTouchEvent), pEvent);
            }
        }
    }
    else if (pEvent->type == TouchEvent::Type_Press)
    {
        pEventStream = NewStream(EventStream::Type_Touch, pEvent->id);

        if (m_gestureMode)
        {
            bool accept;
            m_pActionEngine->Execute(BOI_AEOP(QueryAcceptTouchStream), &accept);

            if (accept)
            {
                pEventStream->target = EventStream::Target_Action;
                pEventStream->touchEvent = *pEvent;

                m_pActionEngine->Execute(BOI_AEOP(SendTouchEvent), pEvent);
            }
        }
        else
        {
            bool eventHandled = false;

            if (m_immediateMode)
            {
                /* 
                 * If there is a component in the overlay or system layer at the
                 * touch event location then send the touch event to this component.
                 */

                QPoint point(pEvent->x, pEvent->y);

                int viewLayerIds = (ViewLayerId_System | ViewLayerId_Overlay);
                CRefList crefList = m_pASI->ComponentsAtViewPoint(point, viewLayerIds);

                if (crefList.Count() > 0)
                {
                    CRef cref = crefList.Value(0);
                    Component* pComponent = cref.GetInstance();

                    if (pComponent != NULL)
                    {
                        int flags = pComponent->m_pData->flags;

                        if (flags & ComponentFlag_HandlesKeyEvents)
                        {
                            m_pASI->SetKeyEventHandler(cref);
                        }

                        if (flags & ComponentFlag_HandlesTouchEvents)
                        {
                            pEventStream->target = EventStream::Target_Component;
                            pEventStream->pComponent = pComponent;
                            pEventStream->touchEvent = *pEvent;
                            pEventStream->cref = cref;

                            QPointF mappedPoint = m_pASI->MapFromViewToComponent(pComponent, point);
                            pEvent->x = mappedPoint.x();
                            pEvent->y = mappedPoint.y();

                            m_pISI->PostTouchEventTask(cref, pEvent);

                            eventHandled = true;
                        }
                        else
                        {
                            cref.ReleaseInstance();
                        }
                    }
                }
            }

            if (!eventHandled)
            {
                /*
                 * If there is an active Action that wants this touch
                 * event then send the touch event to the Action.
                 */

                if (m_pActionEngine->HasActiveAction())
                {
                    bool accept;
                    m_pActionEngine->Execute(BOI_AEOP(QueryAcceptTouchStream), &accept);

                    if (accept)
                    {
                        pEventStream->target = EventStream::Target_Action;
                        pEventStream->touchEvent = *pEvent;

                        m_pActionEngine->Execute(BOI_AEOP(SendTouchEvent), pEvent);

                        eventHandled = true;
                    }
                }
            }

            if (!eventHandled)
            {
                /*
                 * The touch event has not been handled yet. If there are any
                 * Components in any of the layers at the touch event location
                 * then send this touch event to the top most Component.
                 */

                QPoint point(pEvent->x, pEvent->y);

                int viewLayerIds = (m_immediateMode) ? 0 : ViewLayerId_System |
                                                           ViewLayerId_Overlay;
                viewLayerIds |= ViewLayerId_Main | ViewLayerId_Underlay;
                CRefList crefList = m_pASI->ComponentsAtViewPoint(point, viewLayerIds);

                if (crefList.Count() > 0)
                {
                    CRef cref = crefList.Value(0);
                    Component* pComponent = cref.GetInstance();

                    if (pComponent != NULL)
                    {
                        int flags = pComponent->m_pData->flags;

                        if (flags & ComponentFlag_HandlesKeyEvents)
                        {
                            m_pASI->SetKeyEventHandler(cref);
                        }

                        if (flags & ComponentFlag_HandlesTouchEvents)
                        {
                            pEventStream->target = EventStream::Target_Component;
                            pEventStream->pComponent = pComponent;
                            pEventStream->touchEvent = *pEvent;
                            pEventStream->cref = cref;

                            QPointF mappedPoint = m_pASI->MapFromViewToComponent(pComponent, point);
                            pEvent->x = mappedPoint.x();
                            pEvent->y = mappedPoint.y();

                            m_pISI->PostTouchEventTask(cref, pEvent);

                            eventHandled = true;
                        }
                        else
                        {
                            cref.ReleaseInstance();
                        }
                    }
                }
            }
        }
    }
    else if (pEvent->type == TouchEvent::Type_Release)
    {
        pEventStream = GetStream(EventStream::Type_Touch, pEvent->id);

        if (pEventStream->target == EventStream::Target_Component)
        {
            QPoint point(pEvent->x, pEvent->y);
            QPointF mappedPoint = m_pASI->MapFromViewToComponent(pEventStream->pComponent, point);
            pEvent->x = mappedPoint.x();
            pEvent->y = mappedPoint.y();

            m_pISI->PostTouchEventTask(pEventStream->cref, pEvent);

            pEventStream->pComponent = NULL;
            pEventStream->cref.ReleaseInstance();
            pEventStream->cref.Reset();
        }
        else if (pEventStream->target == EventStream::Target_Action)
        {
            m_pActionEngine->Execute(BOI_AEOP(SendTouchEvent), pEvent);
        }

        DeleteStream(pEventStream);
    }
}


void InputEventHandler::HandleKeyEvent(KeyEvent* pEvent)
{
    EventStream* pEventStream;

    if (pEvent->type == KeyEvent::Type_Press)
    {
        pEventStream = NewStream(EventStream::Type_Key, pEvent->id);

        bool eventHandled = false;

        if (m_pActionEngine->HasActiveAction())
        {
            bool accept;
            m_pActionEngine->Execute(BOI_AEOP(QueryAcceptKeyStream), &accept);

            if (accept)
            {
                pEventStream->target = EventStream::Target_Action;
                pEventStream->keyEvent = *pEvent;

                m_pActionEngine->Execute(BOI_AEOP(SendKeyEvent), pEvent);

                eventHandled = true;
            }
        }

        if (!eventHandled)
        {
            CRef cref = m_pASI->KeyEventHandler();

            if (cref.IsValid() && !cref.IsDestroyed())
            {
                pEventStream->target = EventStream::Target_Component;
                pEventStream->keyEvent = *pEvent;
                pEventStream->cref = cref;

                m_pISI->PostKeyEventTask(cref, pEvent);
            }
        }
    }
    else if (pEvent->type == KeyEvent::Type_Release)
    {
        pEventStream = GetStream(EventStream::Type_Key, pEvent->id);

        /*
         * Sometimes the OS sends lone key release events when
         * the application becomes the foreground window. It seems
         * to occur after typing in another window and switching
         * back to this application. This causes GetStream(...)
         * to return NULL which is why the following 'if' exists.
         */
        if (pEventStream != NULL)
        {
            if (pEventStream->target == EventStream::Target_Component)
            {
                m_pISI->PostKeyEventTask(pEventStream->cref, pEvent);

                pEventStream->cref.Reset();
            }
            else if (pEventStream->target == EventStream::Target_Action)
            {
                m_pActionEngine->Execute(BOI_AEOP(SendKeyEvent), pEvent);
            }

            DeleteStream(pEventStream);
        }
    }
}


void InputEventHandler::Destroy()
{
    FlushStreams(true);
}


InputEventHandler::EventStream* InputEventHandler::NewStream(int type, int id)
{
    int index = m_availableEventStreams[m_availableEventStreamsTop];
    m_availableEventStreamsTop++;

    EventStream* pEventStream = &m_eventStreams[index];

    if (m_pEventStreamsHead != NULL)
    {
        m_pEventStreamsHead->pPrev = pEventStream;
    }

    pEventStream->pPrev = NULL;
    pEventStream->pNext = m_pEventStreamsHead;
    m_pEventStreamsHead = pEventStream;

    pEventStream->id = id;
    pEventStream->type = (EventStream::Type)type;
    pEventStream->target = EventStream::Target_None;
    pEventStream->pComponent = NULL;

    return pEventStream;
}


InputEventHandler::EventStream* InputEventHandler::GetStream(int type, int id)
{
    EventStream* pEventStream = m_pEventStreamsHead;

    while (pEventStream != NULL)
    {
        if ((pEventStream->id == id) &&
            (pEventStream->type == type))
        {
            return pEventStream;
        }

        pEventStream = pEventStream->pNext;
    }

    return NULL;
}


void InputEventHandler::DeleteStream(EventStream* pEventStream)
{
    if (pEventStream->pPrev != NULL)
    {
        pEventStream->pPrev->pNext = pEventStream->pNext;
    }
    else
    {
        m_pEventStreamsHead = pEventStream->pNext;
    }

    if (pEventStream->pNext != NULL)
    {
        pEventStream->pNext->pPrev = pEventStream->pPrev;
    }

    m_availableEventStreamsTop--;
    m_availableEventStreams[m_availableEventStreamsTop] = pEventStream->index;
}


/*
 * Actions do not get a chance to handle the 'Type_Release' part
 * of an EventStream since an Action could potentially request
 * something else to happen by returning something other than
 * CONTINUE after processing the event.
 *
 * FlushStreams(...) does not delete the EventStreams because they
 * need to remain active since the corresponding events will
 * still need to be handled after they are flushed. This is why
 * the target for each flushed EventStream is set to Target_None.
 */
void InputEventHandler::FlushStreams(bool fullFlush)
{
    EventStream* pEventStream = m_pEventStreamsHead;

    while (pEventStream != NULL)
    {
        if (pEventStream->type == EventStream::Type_Touch)
        {
            if (pEventStream->target == EventStream::Target_Component)
            {
                if (fullFlush)
                {
                    TouchEvent* pTouchEvent = &pEventStream->touchEvent;

                    QPoint point(pTouchEvent->x, pTouchEvent->y);
                    QPointF mappedPoint = m_pASI->MapFromViewToComponent(pEventStream->pComponent, point);

                    pTouchEvent->x = mappedPoint.x();
                    pTouchEvent->y = mappedPoint.y();
                    pTouchEvent->type = TouchEvent::Type_Release;

                    m_pISI->PostTouchEventTask(pEventStream->cref, pTouchEvent);

                    pEventStream->target = EventStream::Target_None;

                    pEventStream->pComponent = NULL;
                    pEventStream->cref.ReleaseInstance();
                    pEventStream->cref.Reset();
                }
            }
            else if (pEventStream->target == EventStream::Target_Action)
            {
                pEventStream->target = EventStream::Target_None;
            }
        }
        else if (pEventStream->type == EventStream::Type_Key)
        {
            if (pEventStream->target == EventStream::Target_Component)
            {
                KeyEvent* pKeyEvent = &pEventStream->keyEvent;
                pKeyEvent->type = KeyEvent::Type_Release;

                m_pISI->PostKeyEventTask(pEventStream->cref, pKeyEvent);

                pEventStream->cref.Reset();
            }

            pEventStream->target = EventStream::Target_None;
        }

        pEventStream = pEventStream->pNext;
    }
}


void InputEventHandler::HandleActionChanged()
{
    if (m_handleActionChanged)
    {
        FlushStreams(false);

        if (m_gestureMode)
        {
            m_gestureMode = false;
        }
    }
}


} // namespace BOI

