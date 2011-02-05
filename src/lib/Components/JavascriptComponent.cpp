/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QVariant>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "CSI.h"
#include "Events/TouchEvent.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/BasicComponentDrawData.h"
#include "Components/JavascriptComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(JavascriptComponent)
    BOI_DECLARE_RECEIVER("{7f2249e4-6c3c-40a5-9cff-59501f06ee37}",
                         BOI_RECEIVER_FUNC(JavascriptComponent, SetText))
    BOI_DECLARE_RECEIVER("{7f26d820-effc-4fe3-8553-1a57cd88244d}",
                         BOI_RECEIVER_FUNC(JavascriptComponent, ReceiverFunc1))
    BOI_DECLARE_RECEIVER("{eff42c37-57dd-4f1f-95b2-16549eff87a8}",
                         BOI_RECEIVER_FUNC(JavascriptComponent, ReceiverFunc2))
    BOI_DECLARE_RECEIVER("{4bf972ee-26d3-4afe-b38b-71d93c1bb514}",
                         BOI_RECEIVER_FUNC(JavascriptComponent, ReceiverFunc3))
    BOI_DECLARE_RECEIVER("{c00548bc-8792-48b9-a21f-4393b790da17}",
                         BOI_RECEIVER_FUNC(JavascriptComponent, ReceiverFunc4))
BOI_END_RECEIVERS(JavascriptComponent)


BOI_BEGIN_EMITTERS(JavascriptComponent)
    BOI_DECLARE_EMITTER("{24532bc9-ee2c-490c-b519-ea25844f32a0}")
    BOI_DECLARE_EMITTER("{1123ddac-cf4b-4c3b-a0b2-0d1de01ed917}")
    BOI_DECLARE_EMITTER("{a803a200-109e-4511-a9cf-f13d2a62da70}")
    BOI_DECLARE_EMITTER("{0f2b6164-bef3-4e2e-b2a1-754cf2e17dd4}")
BOI_END_EMITTERS(JavascriptComponent)


BOI_BEGIN_FUNCSETS(JavascriptComponent)
BOI_END_FUNCSETS(JavascriptComponent)


BOI_BEGIN_CALLERS(JavascriptComponent)
BOI_END_CALLERS(JavascriptComponent)


JavascriptComponent::JavascriptComponent(const BasicComponentDrawData* pDrawData)
    : Component(BOI_STD_C(Javascript)),
      m_scriptEngine(),
      m_pDrawData(pDrawData)
{
}


bool JavascriptComponent::Initialize()
{
    m_scriptEngine.setProperty("pComponent", qVariantFromValue((void*)this));
    QScriptValue boiObject = m_scriptEngine.evaluate("BOI = new Object();");

    QScriptValue emitFunc1 = m_scriptEngine.newFunction(JavascriptComponent::Emit_1);
    boiObject.setProperty("Emit_1", emitFunc1);

    QScriptValue emitFunc2 = m_scriptEngine.newFunction(JavascriptComponent::Emit_2);
    boiObject.setProperty("Emit_2", emitFunc2);

    QScriptValue emitFunc3 = m_scriptEngine.newFunction(JavascriptComponent::Emit_3);
    boiObject.setProperty("Emit_3", emitFunc3);

    QScriptValue emitFunc4 = m_scriptEngine.newFunction(JavascriptComponent::Emit_4);
    boiObject.setProperty("Emit_4", emitFunc4);

    boiObject.setProperty("DataType_Int", BOI_STD_D(Int));
    boiObject.setProperty("DataType_Bool", BOI_STD_D(Bool));
    boiObject.setProperty("DataType_Point", BOI_STD_D(Point));
    boiObject.setProperty("DataType_Double", BOI_STD_D(Double));
    boiObject.setProperty("DataType_String", BOI_STD_D(String));

    SetBoundingRect(m_pDrawData->BoundingRect());
    SetFlag(ComponentFlag_HandlesTouchEvents);
    return true;
}


void JavascriptComponent::Destroy()
{
}


void JavascriptComponent::HandleTouchEvent(TouchEvent* pEvent)
{
    QScriptValue func = m_scriptEngine.evaluate("BOI.HandleTouchEvent");

    if (func.isFunction())
    {
        QScriptValue array = m_scriptEngine.newArray(4);
        array.setProperty(0, pEvent->x);
        array.setProperty(1, pEvent->y);
        array.setProperty(2, pEvent->id);
        array.setProperty(3, pEvent->type);

        QScriptValueList args;
        args << array;
        func.call(QScriptValue(), args);
    }
}


void JavascriptComponent::Draw(QPainter* pPainter,
                               const QStyleOptionGraphicsItem* pOption)
{
    pPainter->drawImage(pOption->exposedRect,
                        m_pDrawData->Image(),
                        pOption->exposedRect);
}


void JavascriptComponent::SetText(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(String))
    {
        QString text = *dref.GetReadInstance<QString>();
        m_scriptEngine.evaluate(text);
    }
}


void JavascriptComponent::ReceiverFunc1(DRef& dref, int source)
{
    Q_UNUSED(source);

    CallReceiver("BOI.ReceiverFunc1", dref);
}


void JavascriptComponent::ReceiverFunc2(DRef& dref, int source)
{
    Q_UNUSED(source);

    CallReceiver("BOI.ReceiverFunc2", dref);
}


void JavascriptComponent::ReceiverFunc3(DRef& dref, int source)
{
    Q_UNUSED(source);

    CallReceiver("BOI.ReceiverFunc3", dref);
}


void JavascriptComponent::ReceiverFunc4(DRef& dref, int source)
{
    Q_UNUSED(source);

    CallReceiver("BOI.ReceiverFunc4", dref);
}


QScriptValue JavascriptComponent::Emit_1(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QVariant var = pEngine->property("pComponent");
    JavascriptComponent* pComponent = (JavascriptComponent*)var.value<void*>();
    pComponent->Emit(pContext, JavascriptComponent::Emitter_1);

    return QScriptValue(true);
}


QScriptValue JavascriptComponent::Emit_2(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QVariant var = pEngine->property("pComponent");
    JavascriptComponent* pComponent = (JavascriptComponent*)var.value<void*>();
    pComponent->Emit(pContext, JavascriptComponent::Emitter_2);

    return QScriptValue(true);
}


QScriptValue JavascriptComponent::Emit_3(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QVariant var = pEngine->property("pComponent");
    JavascriptComponent* pComponent = (JavascriptComponent*)var.value<void*>();
    pComponent->Emit(pContext, JavascriptComponent::Emitter_3);

    return QScriptValue(true);
}


QScriptValue JavascriptComponent::Emit_4(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QVariant var = pEngine->property("pComponent");
    JavascriptComponent* pComponent = (JavascriptComponent*)var.value<void*>();
    pComponent->Emit(pContext, JavascriptComponent::Emitter_4);

    return QScriptValue(true);
}


void JavascriptComponent::CallReceiver(const QString& funcName, DRef& dref)
{
    QScriptValue scriptValue;

    QScriptValue func = m_scriptEngine.evaluate(funcName);
    if (func.isFunction())
    {
        if (Convert(dref, scriptValue))
        {
            QScriptValueList args;
            args << scriptValue;
            func.call(QScriptValue(), args);
        }
        else
        {
            func.call();
        }
    }
}


void JavascriptComponent::Emit(QScriptContext *pContext, int emitter)
{
    if (EmitterConnected(emitter))
    {
        if (pContext->argumentCount() > 1)
        {
            QScriptValue arg0 = pContext->argument(0);
            QScriptValue arg1 = pContext->argument(1);

            if (arg1.isNumber())
            {
                int type = arg1.toInt32();

                if (type == BOI_STD_D(Int))
                {
                    int number = arg0.toInt32();

                    DRef dref = SI()->NewData(BOI_STD_D(Int));
                    *dref.GetWriteInstance<int>() = number;

                    Component::Emit(emitter, dref, true);
                }
                else if (type == BOI_STD_D(String))
                {
                    QString string = arg0.toString();

                    DRef dref = SI()->NewData(BOI_STD_D(String));
                    *dref.GetWriteInstance<QString>() = string;

                    Component::Emit(emitter, dref, true);
                }
                else if (type == BOI_STD_D(Point))
                {
                    QVariant var = arg0.toVariant();

                    if (var.isValid())
                    {
                        QVariantList list = var.toList();

                        if (list.size() > 1)
                        {
                            DRef dref = SI()->NewData(BOI_STD_D(Point));
                            QPointF* pPoint = dref.GetWriteInstance<QPointF>();
                            pPoint->setX(list.at(0).toReal());
                            pPoint->setY(list.at(1).toReal());

                            Component::Emit(emitter, dref, true);
                        }
                    }
                }
            }
        }
    }
}


bool JavascriptComponent::Convert(DRef& dref, QScriptValue& scriptValue)
{
    int type = dref.Type();

    if (type == BOI_STD_D(String))
    {
        QString string = *dref.GetReadInstance<QString>();
        scriptValue = QScriptValue(string);
    }
    else if (type == BOI_STD_D(Int))
    {
        int number = *dref.GetReadInstance<int>();
        scriptValue = QScriptValue(number);
    }
    else if (type == BOI_STD_D(Float))
    {
        float number = *dref.GetReadInstance<float>();
        scriptValue = QScriptValue(number);
    }
    else if (type == BOI_STD_D(Double))
    {
        double number = *dref.GetReadInstance<double>();
        scriptValue = QScriptValue(number);
    }
    else if (type == BOI_STD_D(Bool))
    {
        bool boolean = *dref.GetReadInstance<bool>();
        scriptValue = QScriptValue(boolean);
    }
    else if (type == BOI_STD_D(Point))
    {
        QPointF point = *dref.GetReadInstance<QPointF>();

        scriptValue = m_scriptEngine.newArray(2);
        scriptValue.setProperty(0, point.x());
        scriptValue.setProperty(1, point.y());
    }
    else
    {
        return false;
    }

    return true;
}


} // namespace BOI

