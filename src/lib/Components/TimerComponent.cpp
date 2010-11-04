/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QString>
#include "CSI.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/BasicComponentDrawData.h"
#include "Components/TimerComponent.h"


namespace BOI {


TimerObject::TimerObject(TimerComponent* pComponent)
    : QObject(),
      m_timerId(0),
      m_pTimerComponent(pComponent),
      m_doEmit(true)
{
    pComponent->SI()->RegisterMutex(&m_emitLock);
}


void TimerObject::Start(int msec)
{
    if (m_timerId != 0)
    {
        killTimer(m_timerId);
    }

    m_timerId = startTimer(msec);
}


void TimerObject::Stop(bool destroy)
{
    if (m_timerId != 0)
    {
        killTimer(m_timerId);
    }

    if (destroy)
    {
        delete this;
    }
}


void TimerObject::timerEvent(QTimerEvent *pEvent)
{
    Q_UNUSED(pEvent);

    m_emitLock.Lock();

    if (m_doEmit)
    {
        m_pTimerComponent->EmitTimeout();
    }

    m_emitLock.Unlock();
}


void TimerObject::HandleComponentDestroyed()
{
    m_emitLock.Lock();
    m_doEmit = false;
    m_emitLock.Unlock();
}


TimerObjectController::TimerObjectController()
    : QObject()
{
}


void TimerObjectController::StartTimer(int msec)
{
    emit Start(msec);
}


void TimerObjectController::StopTimer(bool destroy)
{
    emit Stop(destroy);
}


BOI_BEGIN_RECEIVERS(TimerComponent)
    BOI_DECLARE_RECEIVER("{898b92cc-bec5-42f3-ae9f-8dbd2b0b8af8}",
                         BOI_RECEIVER_FUNC(TimerComponent, StartTimer))
    BOI_DECLARE_RECEIVER("{4be3297a-aea2-4e3c-85ef-6ca8dcb4fa10}",
                         BOI_RECEIVER_FUNC(TimerComponent, StopTimer))
BOI_END_RECEIVERS(TimerComponent)


BOI_BEGIN_EMITTERS(TimerComponent)
    BOI_DECLARE_EMITTER("{8eff1256-171c-44b5-82a3-fa64d3625c31}")
BOI_END_EMITTERS(TimerComponent)


BOI_BEGIN_FUNCSETS(TimerComponent)
BOI_END_FUNCSETS(TimerComponent)


BOI_BEGIN_CALLERS(TimerComponent)
BOI_END_CALLERS(TimerComponent)


TimerComponent::TimerComponent(QThread* pThread,
                               const BasicComponentDrawData* pDrawData)
    : Component(BOI_STD_C(Timer)),
      m_pThread(pThread),
      m_controller(),
      m_pDrawData(pDrawData)
{
}


bool TimerComponent::Initialize()
{
    m_pTimerObject = new TimerObject(this);
    m_pTimerObject->moveToThread(m_pThread);

    QObject::connect(&m_controller,  SIGNAL(Start(int)),
                     m_pTimerObject, SLOT(Start(int)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(Stop(bool)),
                     m_pTimerObject, SLOT(Stop(bool)),
                     Qt::QueuedConnection);

    SetBoundingRect(m_pDrawData->BoundingRect());

    return true;
}


void TimerComponent::Destroy()
{
    m_pTimerObject->HandleComponentDestroyed();

    m_controller.StopTimer(true);
}


void TimerComponent::Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption)
{
    pPainter->drawImage(pOption->exposedRect,
                        m_pDrawData->Image(),
                        pOption->exposedRect);
}


void TimerComponent::StartTimer(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(String))
    {
        QString string = *dref.GetReadInstance<QString>();

        bool ok;
        int msec = string.toInt(&ok);
        if (ok)
        {
            m_controller.StartTimer(msec);
        }
    }
    else if (dref.Type() == BOI_STD_D(Int))
    {
        int msec = *dref.GetReadInstance<int>();
        m_controller.StartTimer(msec);
    }
}


void TimerComponent::StopTimer(DRef& dref, int source)
{
    Q_UNUSED(dref);
    Q_UNUSED(source);

    m_controller.StopTimer(false);
}


void TimerComponent::EmitTimeout()
{
    if (EmitterConnected(Emitter_Timeout))
    {
        DRef dref;
        Emit(Emitter_Timeout, dref, true);
    }
}


} // namespace BOI

