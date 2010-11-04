/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __COMPONENTSFACTORY_H
#define __COMPONENTSFACTORY_H


#include "Factory.h"


class ComponentsFactory
    : public BOI::Factory
{
    public:
        ComponentsFactory();

        virtual void RegisterTypes(BOI::TypeRegistrar* pRegistrar);

        DEFINE_BOI_GET_INSTANCE_FUNC(ComponentsFactory, GetComponent);

    private:
        int m_rectComponentType;
};


#endif //__COMPONENTSFACTORY_H

