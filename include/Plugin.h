/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_PLUGIN_H
#define __BOI_PLUGIN_H


#include <QtPlugin>


namespace BOI {


class MenuRegistrar;
class FactoryRegistrar;
class DescriptionRegistrar;


class Plugin
{
    public:
        virtual ~Plugin() {}

        virtual void RegisterMenus(MenuRegistrar* pRegistrar) = 0;

        virtual void RegisterDescriptions(DescriptionRegistrar* pRegistrar) = 0;

        virtual void RegisterDataFactories(FactoryRegistrar*      pRegistrar) = 0;
        virtual void RegisterActionFactories(FactoryRegistrar*    pRegistrar) = 0;
        virtual void RegisterComponentFactories(FactoryRegistrar* pRegistrar) = 0;
};


} // namespace BOI


Q_DECLARE_INTERFACE(BOI::Plugin, "BOI.Plugin/1.0");


#endif //__BOI_PLUGIN_H

