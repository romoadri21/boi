/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_INPUTEVENTHANDLER_H
#define __BOI_INPUTEVENTHANDLER_H


#include "Events/InputModeChangeEventListener.h"
#include "Events/TouchEventListener.h"
#include "Events/KeyEventListener.h"
#include "ActionChangedListener.h"
#include "Events/TouchEvent.h"
#include "Events/KeyEvent.h"
#include "CRef.h"


#define BOI_MAX_EVENTSTREAMS 32


namespace BOI {


class ASI;
class ISI;
class Component;
class ActionEngine;
class InputModeChangeEvent;


class InputEventHandler
    : public InputModeChangeEventListener,
      public ActionChangedListener,
      public TouchEventListener,
      public KeyEventListener
{
    public:
        InputEventHandler(ActionEngine* pActionEngine);

        void SetASI(ASI* pASI);
        void SetISI(ISI* pISI);

        void HandleInputModeChangeEvent(InputModeChangeEvent* pEvent);

        void HandleTouchEvent(TouchEvent* pEvent);
        void HandleKeyEvent(KeyEvent* pEvent);

        void HandleActionChanged();

        /*
         * When the InputEventHandler will no longer be handling
         * any events (ie. when the program is shutting down),
         * call this method to perform final processing and release
         * any resources (ie. CRefs, etc...) that may be in use.
         */
        void Destroy();

    private:
        class EventStream
        {
            public:
                enum Type
                {
                    Type_Touch = 0,
                    Type_Key
                };

                enum Target
                {
                    Target_None = 0,
                    Target_Action,
                    Target_Component
                };

            public:
                int id;
                Type type;
                Target target;

                union
                {
                    TouchEvent touchEvent;
                    KeyEvent   keyEvent;
                };

                CRef cref;
                Component* pComponent;

                EventStream* pNext;
                EventStream* pPrev;

                int index;
        };

    private:
        EventStream* NewStream(int type, int id);
        EventStream* GetStream(int type, int id);

        void DeleteStream(EventStream* pEventStream);
        void FlushStreams(bool fullFlush);

    private:
        ActionEngine* m_pActionEngine;

        ASI* m_pASI;
        ISI* m_pISI;

        EventStream  m_eventStreams[BOI_MAX_EVENTSTREAMS];
        EventStream* m_pEventStreamsHead;

        int m_availableEventStreams[BOI_MAX_EVENTSTREAMS];
        int m_availableEventStreamsTop;

        bool m_gestureMode;
        bool m_immediateMode;

        bool m_handleActionChanged;
};


} // namespace BOI


#endif //__BOI_INPUTEVENTHANDLER_H

