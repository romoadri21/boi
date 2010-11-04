/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ACTIONMANAGER_H
#define __BOI_ACTIONMANAGER_H


#include <QHash>
#include <QString>
#include "FactoryRegistrar.h"
#include "TypeManager.h"


namespace BOI {


class Action;


class ActionManager
    : public FactoryRegistrar
{
    public:
        ActionManager();

        bool Initialize();
        void RegisterFactory(Factory* pFactory);

        int ConvertUuid(const QString& uuid);
        QString GetUuid(int type);

        Action* GetAction(int type);

        void DestroyActions();

    private:
        TypeManager m_typeManager;

        QHash<int, Action*> m_actions;
};


inline int ActionManager::ConvertUuid(const QString& uuid)
{
    return m_typeManager.ConvertUuid(uuid);
}


inline QString ActionManager::GetUuid(int type)
{
    return m_typeManager.TypeRegistered(type) ?
           m_typeManager.GetUuid(type) :
           QString();
}


} // namespace BOI


#endif //__BOI_ACTIONMANAGER_H

