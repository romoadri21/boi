/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include <QThread>
#include <QSizeF>
#include "TypeRegistrar.h"
#include "StandardComponents.h"
#include "Components/TimerComponent.h"
#include "Components/BasicComponentDrawData.h"
#include "Factories/TimerComponentFactory.h"


namespace BOI {


TimerComponentFactory::TimerComponentFactory()
    : Factory()
{
    // TODO: put the draw data initialization in the component creation
    // part so it's only created when necessary. Use a mutex to stop it
    // from being created at the same time by multiple threads. Or use an
    // atomic pointer?
    m_pTimerDrawData = new BasicComponentDrawData();
    m_pTimerDrawData->SetSvg(":/BOI/Timer.svg", QSizeF(105, 105));

    m_pThread = new QThread();
    m_pThread->start();
}


TimerComponentFactory::~TimerComponentFactory()
{
    m_pThread->quit();
    m_pThread->wait();
    delete m_pThread;

    delete m_pTimerDrawData;
}


void TimerComponentFactory::RegisterTypes(TypeRegistrar* pRegistrar)
{
    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(TimerComponentFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(Timer)));
}


Object* TimerComponentFactory::GetComponent(int type)
{
    Component* pComponent = NULL;

    switch (type)
    {
        case BOI_STD_C(Timer):
            pComponent = new TimerComponent(m_pThread, m_pTimerDrawData);
            break;

        default:
            break;
    }

    return pComponent; 
}


} // namespace BOI

