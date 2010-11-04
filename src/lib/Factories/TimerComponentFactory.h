/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TIMERCOMPONENTFACTORY_H
#define __BOI_TIMERCOMPONENTFACTORY_H


#include "Factory.h"


class QThread;


namespace BOI {


class BasicComponentDrawData;


class TimerComponentFactory
    : public Factory
{
    public:
        TimerComponentFactory();
        ~TimerComponentFactory();

        virtual void RegisterTypes(TypeRegistrar* pRegistrar);

        DEFINE_BOI_GET_INSTANCE_FUNC(TimerComponentFactory, GetComponent);

    private:
        QThread* m_pThread;
        BasicComponentDrawData* m_pTimerDrawData;
};


} // namespace BOI


#endif //__BOI_TIMERCOMPONENTFACTORY_H

