/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TYPEMANAGER_H
#define __BOI_TYPEMANAGER_H


#include <QString>
#include <QHash>
#include "TypeRegistrar.h"


namespace BOI {


class Object;


class TypeManager
    : public TypeRegistrar
{
    public:
        TypeManager(int bucketSize=20);
        ~TypeManager();

        virtual int RegisterType(void*                       pFactory,
                                 Factory::GetInstanceFunc    getInstanceFunc,
                                 Factory::DeleteInstanceFunc deleteInstanceFunc,
                                 const QString&              uuid);

        virtual bool TypeRegistered(int type);
        virtual bool TypeRegistered(const QString& uuid);

        virtual int ConvertUuid(const QString& uuid);
        virtual QString GetUuid(int type);

        virtual Object* GetInstance(int type);
        virtual void DeleteInstance(Object* pInstance);

    private:
        typedef struct
        {
            QString                     uuid;
            void*                       pFactory;
            Factory::GetInstanceFunc    getInstanceFunc;
            Factory::DeleteInstanceFunc deleteInstanceFunc;
        } TypeHandler;

    private:
        TypeHandler* m_pTypeHandlers;
        int m_numTypeHandlers;
        int m_maxTypeHandlers;

        int m_bucketSize;

        /*
         * A QString is used as the key for the hash instead
         * of a QUuid because getting the hash value for a
         * QUuid automatically converts the QUuid to a QString
         * anyway. Thus, using a QString avoids the unnecessary
         * conversion.
         * This does imply that all UUIDs that will be passed
         * into the system follow the same convention so that
         * they can equate correctly. They should all start
         * with a '{', end with a '}' and consist only of
         * lowercase letters and digits. 
         */
        QHash<QString, int> m_registeredTypes;
};


inline bool TypeManager::TypeRegistered(int type)
{
    return (type < m_numTypeHandlers);
}


inline QString TypeManager::GetUuid(int type)
{
    return m_pTypeHandlers[type].uuid;
}


} // namespace BOI


#endif //__BOI_TYPEMANAGER_H

