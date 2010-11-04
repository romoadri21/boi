/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TIMERCOMPONENT_H
#define __BOI_TIMERCOMPONENT_H


#include <QObject>
#include "StandardEmitters.h"
#include "Component.h"
#include "Mutex.h"


class QThread;


namespace BOI {


class TimerComponent;
class BasicComponentDrawData;


class TimerObject
    : public QObject
{
    Q_OBJECT

    public:
        TimerObject(TimerComponent* pComponent);

    public:
        void HandleComponentDestroyed();

    public slots:
        void Start(int msec);
        void Stop(bool destroy);

    protected:
        void timerEvent(QTimerEvent *pEvent);

    private:
        int m_timerId;
        TimerComponent* m_pTimerComponent;

        bool m_doEmit;
        Mutex m_emitLock;
};


class TimerObjectController
    : public QObject
{
    Q_OBJECT

    public:
        TimerObjectController();

        void StartTimer(int msec);
        void StopTimer(bool destroy);

    signals:
        void Start(int msec);
        void Stop(bool destroy);
};


class TimerComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    BOI_DEFINE_RECEIVER_FUNC(TimerComponent, StartTimer);
    BOI_DEFINE_RECEIVER_FUNC(TimerComponent, StopTimer);

    public:
        TimerComponent(QThread* pThread,
                       const BasicComponentDrawData* pDrawData);

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

    private:
        enum
        {
            Emitter_Timeout = StandardEmitters::NumEmitters
        };

    private:
        void EmitTimeout();

    private:
        QThread* m_pThread;

        friend class TimerObject;
        TimerObject* m_pTimerObject;
        TimerObjectController m_controller;

        const BasicComponentDrawData* m_pDrawData;
};


} // namespace BOI


#endif //__BOI_TIMERCOMPONENT_H

