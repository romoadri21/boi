/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COMPONENTMANAGER_H
#define __BOI_COMPONENTMANAGER_H


#include <QHash>
#include <QMutex>
#include <QString>
#include "FactoryRegistrar.h"
#include "TypeManager.h"
#include "ViewLayerId.h"
#include "CRef.h"


namespace BOI {


class CSI;
class ISI;
class ComponentData;


class ComponentManager
    : public FactoryRegistrar
{
    public:
        ComponentManager();
        ~ComponentManager();

        bool Initialize();

        void SetCSI(CSI* pCSI);
        void SetISI(ISI* pISI);

        void RegisterFactory(Factory* pFactory);

        int ConvertUuid(const QString& uuid);
        QString GetUuid(int type);

        CRef NewComponent(int type, ViewLayerId layer=ViewLayerId_Main);

        void DestroyAll(bool stopNew=false);

        bool IsLayout(CRef& cref);

    protected:
        /*
         * These methods should *only* be called from
         * within CRef through ISI.
         */
        friend class ISI;
        void DestroyComponent(ComponentData* pComponentData);
        void DeleteComponent(ComponentData*  pComponentData);

        void AddToList(ComponentData* pComponentData);
        void RemoveFromList(ComponentData* pComponentData);

    private:
        TypeManager m_typeManager;

        CSI* m_pCSI;
        ISI* m_pISI;

        QAtomicInt m_componentId;

        /*
         * The number of components in the system.
         */
        QAtomicInt m_numComponents;

        QAtomicInt m_numNewAccessors;
        const int  MaxNewAccessors;

        QMutex m_mutex;
        ComponentData* m_pHead;

        QHash<int, int> m_layoutTypes;
};


inline int ComponentManager::ConvertUuid(const QString& uuid)
{
    return m_typeManager.ConvertUuid(uuid);
}


inline QString ComponentManager::GetUuid(int type)
{
    return m_typeManager.TypeRegistered(type) ?
           m_typeManager.GetUuid(type) :
           QString();
}


} // namespace BOI


#endif //__BOI_COMPONENTMANAGER_H

