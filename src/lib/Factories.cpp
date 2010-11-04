/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "FactoryRegistrar.h"
#include "Factories/CoreActionsFactory.h"
#include "Factories/CoreDataTypesFactory.h"
#include "Factories/TimerComponentFactory.h"
#include "Factories/StandardComponentsFactory.h"
#include "Factories/PrimitiveTypesDataFactory.h"
#include "Factories.h"


namespace BOI {


Factories::Factories()
    : m_pCoreActionsFactory(NULL),
      m_pCoreDataTypesFactory(NULL),
      m_pTimerComponentFactory(NULL),
      m_pStandardComponentsFactory(NULL),
      m_pPrimitiveTypesDataFactory(NULL)
{
}


Factories::~Factories()
{
    delete m_pCoreActionsFactory;
    delete m_pCoreDataTypesFactory;
    delete m_pTimerComponentFactory;
    delete m_pStandardComponentsFactory;
    delete m_pPrimitiveTypesDataFactory;
}


bool Factories::InitDataFactories(FactoryRegistrar* pRegistrar)
{
    try
    {
        m_pPrimitiveTypesDataFactory = new PrimitiveTypesDataFactory();
        pRegistrar->RegisterFactory(m_pPrimitiveTypesDataFactory);

        m_pCoreDataTypesFactory = new CoreDataTypesFactory();
        pRegistrar->RegisterFactory(m_pCoreDataTypesFactory);
    }
    catch (...)
    {
        return false;
    }

    return true;
}


bool Factories::InitActionFactories(FactoryRegistrar* pRegistrar)
{
    try
    {
        m_pCoreActionsFactory = new CoreActionsFactory();
        pRegistrar->RegisterFactory(m_pCoreActionsFactory);
    }
    catch (...)
    {
        return false;
    }

    return true;
}


bool Factories::InitComponentFactories(FactoryRegistrar* pRegistrar)
{
    try
    {
        m_pStandardComponentsFactory = new StandardComponentsFactory();
        pRegistrar->RegisterFactory(m_pStandardComponentsFactory);

        m_pTimerComponentFactory = new TimerComponentFactory();
        pRegistrar->RegisterFactory(m_pTimerComponentFactory);
    }
    catch (...)
    {
        return false;
    }

    return true;
}


} // namespace BOI

