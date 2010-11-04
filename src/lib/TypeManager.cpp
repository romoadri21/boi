/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "Object.h"
#include "TypeManager.h"


namespace BOI {


TypeManager::TypeManager(int bucketSize)
    : m_pTypeHandlers(NULL),
      m_numTypeHandlers(0),
      m_maxTypeHandlers(0),
      m_bucketSize(bucketSize),
      m_registeredTypes()
{
}


TypeManager::~TypeManager()
{
    delete[] m_pTypeHandlers;
}


int TypeManager::RegisterType(void*                       pFactory,
                              Factory::GetInstanceFunc    getInstanceFunc,
                              Factory::DeleteInstanceFunc deleteInstanceFunc,
                              const QString&              uuid)
{
    int index = m_registeredTypes.value(uuid, m_numTypeHandlers);

    if (index == m_numTypeHandlers)
    {
        /*
         * The type is not already registered so add
         * it to the system.
         */

        if (m_numTypeHandlers == m_maxTypeHandlers)
        {
            /*
             * Make more room for the incoming TypeHandler
             * since the current buffer is full.
             */

            m_maxTypeHandlers += m_bucketSize;
            TypeHandler* pTypeHandlers = new TypeHandler[m_maxTypeHandlers];

            for (int i=0; i < m_numTypeHandlers; i++)
            {
                pTypeHandlers[i] = m_pTypeHandlers[i];
            }

            delete[] m_pTypeHandlers;
            m_pTypeHandlers = pTypeHandlers;
        }

        m_registeredTypes.insert(uuid, index);
        m_numTypeHandlers++;
    }

    m_pTypeHandlers[index].uuid               = uuid;
    m_pTypeHandlers[index].pFactory           = pFactory;
    m_pTypeHandlers[index].getInstanceFunc    = getInstanceFunc;
    m_pTypeHandlers[index].deleteInstanceFunc = deleteInstanceFunc;

    return index;
}


bool TypeManager::TypeRegistered(const QString& uuid)
{
    return m_registeredTypes.contains(uuid);
}


int TypeManager::ConvertUuid(const QString& uuid)
{
    return m_registeredTypes.value(uuid, -1);
}


Object* TypeManager::GetInstance(int type)
{
    void* pFactory = m_pTypeHandlers[type].pFactory;
    return m_pTypeHandlers[type].getInstanceFunc(pFactory, type);
}


void TypeManager::DeleteInstance(Object* pInstance)
{
    int type = pInstance->Type();
    Factory::DeleteInstanceFunc deleteInstanceFunc = m_pTypeHandlers[type].deleteInstanceFunc;

    if (deleteInstanceFunc != NULL)
    {
        void* pFactory = m_pTypeHandlers[type].pFactory;
        deleteInstanceFunc(pFactory, pInstance);
    }
    else
    {
        delete pInstance;
    }
}


} // namespace BOI

