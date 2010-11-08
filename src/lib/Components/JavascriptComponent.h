/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_JAVASCRIPTCOMPONENT_H
#define __BOI_JAVASCRIPTCOMPONENT_H


#include <QScriptValue>
#include <QScriptEngine>
#include "StandardEmitters.h"
#include "Component.h"


class QScriptContext;


namespace BOI {


class BasicComponentDrawData;


class JavascriptComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();
    BOI_DEFINE_RECEIVER_FUNC(JavascriptComponent, SetText);
    BOI_DEFINE_RECEIVER_FUNC(JavascriptComponent, ReceiverFunc1);
    BOI_DEFINE_RECEIVER_FUNC(JavascriptComponent, ReceiverFunc2);
    BOI_DEFINE_RECEIVER_FUNC(JavascriptComponent, ReceiverFunc3);
    BOI_DEFINE_RECEIVER_FUNC(JavascriptComponent, ReceiverFunc4);

    public:
        JavascriptComponent(const BasicComponentDrawData* pDrawData);

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual bool HandlesTouchEvents();
        virtual void HandleTouchEvent(TouchEvent* pEvent);

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

    private:
        enum
        {
            Emitter_1 = StandardEmitters::NumEmitters,
            Emitter_2,
            Emitter_3,
            Emitter_4
        };

    private:
        static QScriptValue Emit_1(QScriptContext *pContext, QScriptEngine *pEngine);
        static QScriptValue Emit_2(QScriptContext *pContext, QScriptEngine *pEngine);
        static QScriptValue Emit_3(QScriptContext *pContext, QScriptEngine *pEngine);
        static QScriptValue Emit_4(QScriptContext *pContext, QScriptEngine *pEngine);

        void CallReceiver(const QString& funcName, DRef& dref);
        void Emit(QScriptContext *pContext, int emitter);

        bool Convert(DRef& dref, QScriptValue& scriptValue);

    private:
        QScriptEngine m_scriptEngine;

        const BasicComponentDrawData* m_pDrawData;
};


} // namespace BOI


#endif //__BOI_JAVASCRIPTCOMPONENT_H

