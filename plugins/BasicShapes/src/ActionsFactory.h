/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __ACTIONSFACTORY_H
#define __ACTIONSFACTORY_H


#include "Factory.h"


class ActionsFactory
    : public BOI::Factory
{
    public:
        ActionsFactory();

        virtual void RegisterTypes(BOI::TypeRegistrar* pRegistrar);

        DEFINE_BOI_GET_INSTANCE_FUNC(ActionsFactory, GetAction);

    private:
        int m_insertActionTypeId;
};


#endif //__ACTIONSFACTORY_H

