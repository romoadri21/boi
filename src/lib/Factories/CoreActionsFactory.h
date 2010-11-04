/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COREACTIONSFACTORY_H
#define __BOI_COREACTIONSFACTORY_H


#include "Factory.h"


namespace BOI {


class CoreActionsFactory
    : public Factory
{
    public:
        CoreActionsFactory();

        virtual void RegisterTypes(TypeRegistrar* pRegistrar);

        DEFINE_BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction);
};


} // namespace BOI


#endif //__BOI_COREACTIONSFACTORY_H

