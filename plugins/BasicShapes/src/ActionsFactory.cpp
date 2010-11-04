/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "InsertShapeAction.h"
#include "TypeRegistrar.h"
#include "ActionsFactory.h"


ActionsFactory::ActionsFactory()
    : Factory()
{
}


void ActionsFactory::RegisterTypes(BOI::TypeRegistrar* pRegistrar)
{
    m_insertActionTypeId = 
        pRegistrar->RegisterType(this,
                                 BOI_GET_INSTANCE_FUNC(ActionsFactory, GetAction),
                                 NULL,
                                 "{cbbbdce0-542f-40a4-a558-79c02e7ec88f}");
}


BOI::Object* ActionsFactory::GetAction(int type)
{
    BOI::Action* pAction = NULL;

    if (type == m_insertActionTypeId)
    {
        pAction = new InsertShapeAction(m_insertActionTypeId);
    }

    return pAction; 
}

