/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "RectComponent.h"
#include "TypeRegistrar.h"
#include "ComponentsFactory.h"


ComponentsFactory::ComponentsFactory()
    : Factory()
{
}


void ComponentsFactory::RegisterTypes(BOI::TypeRegistrar* pRegistrar)
{
    m_rectComponentType =
        pRegistrar->RegisterType(this,
                                 BOI_GET_INSTANCE_FUNC(ComponentsFactory, GetComponent),
                                 NULL,
                                 "{5899283b-cfd6-4dfc-9544-4a4608d27fd9}");
}


BOI::Object* ComponentsFactory::GetComponent(int type)
{
    BOI::Component* pComponent = NULL;

    if (type == m_rectComponentType)
    {
        pComponent = new RectComponent(m_rectComponentType);
    }

    return pComponent;
}

