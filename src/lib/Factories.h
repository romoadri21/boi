/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_FACTORIES_H
#define __BOI_FACTORIES_H


namespace BOI {


class FactoryRegistrar;
class CoreActionsFactory;
class CoreDataTypesFactory;
class TimerComponentFactory;
class StandardComponentsFactory;
class PrimitiveTypesDataFactory;


/*
 * Manager for the built-in factories.
 */

class Factories
{
    public:
        Factories();
        ~Factories();

        bool InitDataFactories(FactoryRegistrar*      pRegistrar);
        bool InitActionFactories(FactoryRegistrar*    pRegistrar);
        bool InitComponentFactories(FactoryRegistrar* pRegistrar);

    private:
        CoreActionsFactory*        m_pCoreActionsFactory;
        CoreDataTypesFactory*      m_pCoreDataTypesFactory;
        TimerComponentFactory*     m_pTimerComponentFactory;
        StandardComponentsFactory* m_pStandardComponentsFactory;
        PrimitiveTypesDataFactory* m_pPrimitiveTypesDataFactory;
};


} // namespace BOI


#endif //__BOI_FACTORIES_H

