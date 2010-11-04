/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "Action.h"
#include "StandardActions.h"
#include "ActionManager.h"


namespace BOI {


ActionManager::ActionManager()
    : FactoryRegistrar(),
      m_typeManager(),
      m_actions()
{
}


bool ActionManager::Initialize()
{
    /*
     * Reserve space and the matching system internal
     * type for the built-in actions.
     */

    for (int i=0; i < StandardActions::NumActions; i++)
    {
        m_typeManager.RegisterType(NULL,
                                   NULL,
                                   NULL,
                                   StandardActions::Uuid(i));
    }

    return true;
}


void ActionManager::RegisterFactory(Factory* pFactory)
{
    pFactory->RegisterTypes(&m_typeManager);
}


Action* ActionManager::GetAction(int type)
{
    Action* pAction = m_actions.value(type, NULL);

    if ((pAction == NULL) && m_typeManager.TypeRegistered(type))
    {
        pAction = (Action*)m_typeManager.GetInstance(type);
        m_actions.insert(type, pAction);
    }

    return pAction;
}


void ActionManager::DestroyActions()
{
    QHashIterator<int, Action*> it(m_actions);
    while (it.hasNext())
    {
        it.next();
        it.value()->Destroy();
        m_typeManager.DeleteInstance(it.value());
    }
}


} // namespace BOI

