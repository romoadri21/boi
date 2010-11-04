/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BASICSHAPESPLUGIN_H
#define __BASICSHAPESPLUGIN_H


#include <QObject>
#include "Plugin.h"
#include "ActionsFactory.h"
#include "ComponentsFactory.h"


class BasicShapesPlugin
    : public QObject,
      public BOI::Plugin
{
    Q_OBJECT
    Q_INTERFACES(BOI::Plugin)

    public:
        virtual void RegisterMenus(BOI::MenuRegistrar* pRegistrar);

        virtual void RegisterDescriptions(BOI::DescriptionRegistrar* pRegistrar);

        virtual void RegisterDataFactories(BOI::FactoryRegistrar*      pRegistrar);
        virtual void RegisterActionFactories(BOI::FactoryRegistrar*    pRegistrar);
        virtual void RegisterComponentFactories(BOI::FactoryRegistrar* pRegistrar);

    private:
        ActionsFactory m_actionsFactory;
        ComponentsFactory m_componentsFactory;
};


#endif // __BASICSHAPESPLUGIN_H

