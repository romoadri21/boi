/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_GUIREQUESTHANDLER_H
#define __BOI_GUIREQUESTHANDLER_H


#include <QQueue>
#include <QMutex>
#include "GuiRequest.h"
#include "GuiRequestPool.h"
#include "Events/GuiRequestEventListener.h"


#define DEFINE_BOI_GUIREQUESTHANDLER_FUNC(FunctionName) \
    static inline void FunctionName ## _Static(GuiRequestHandler* pHandler, GuiRequest* pRequest) \
    { \
        ((GuiRequestHandler*)pHandler)->FunctionName(pRequest); \
    } \
    void FunctionName(GuiRequest* pRequest)

#define BOI_GUIREQUESTHANDLER_FUNC(FunctionName) \
    GuiRequestHandler::FunctionName##_Static


class QObject;


namespace BOI {


class View;
class Component;
class CustomEventsFactory;


class GuiRequestHandler
    : public GuiRequestEventListener
{
    public:
        GuiRequestHandler();

        void SetCustomEventsFactory(CustomEventsFactory* pFactory);
        void SetEventReceiver(QObject* pReceiver);
        void SetView(View* pView);

        bool PostRequest(GuiRequest* pRequest);

        virtual void HandleGuiRequestEvent();

        /*
         * Note, this function should only be called when
         * the application is being shutdown and the main
         * event loop has already been stopped.
         */
        void FlushRequests();

    private:
        typedef void (*GuiRequestFunc) (GuiRequestHandler* pGuiRequestHandler,
                                        GuiRequest*        pRequest);

    private:
        void ProcessRequests();

        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(AddToLayer);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(DestroyComponent);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(SetBoundingRect);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(CenterComponentOn);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(SetTransformOrigin);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(SetRotation);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(SetPosition);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(StackOnTop);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(SetVisible);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(SetOpacity);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(SetParent);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(UpdateRect);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(Update);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(Emit);
        DEFINE_BOI_GUIREQUESTHANDLER_FUNC(Rotate);

        void EmitParentBoundingBox(Component* pComponent, bool newOnly);

    private:
        GuiRequestFunc m_funcs[GuiRequest::NumRequests];

        GuiRequestPool m_guiRequestPool;

        QQueue<GuiRequest*> m_requestQueue;
        GuiRequest* m_pRequestsTail;
        int m_numRequests;
        QMutex m_mutex;

        const int MaxRequestsPerEvent;

        CustomEventsFactory* m_pCustomEventsFactory;
        QObject* m_pEventReceiver;

        View* m_pView;
};


} // namespace BOI


#endif //__BOI_GUIREQUESTHANDLER_H

