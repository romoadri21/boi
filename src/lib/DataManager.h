/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_DATAMANAGER_H
#define __BOI_DATAMANAGER_H


#include <QString>
#include "FactoryRegistrar.h"
#include "TypeManager.h"
#include "DRef.h"


class QDataStream;


namespace BOI {


class Data;


class DataManager
    : public FactoryRegistrar
{
    public:
        DataManager();

        bool Initialize();

        void RegisterFactory(Factory* pFactory);

        int ConvertUuid(const QString& uuid);
        QString GetUuid(int type);

        DRef GetData(int type);

        /*
         * Exports and Imports Data objects to
         * and from the passed in QDataStream.
         */
        DRef Import(QDataStream& in);
        void Export(DRef& dref, QDataStream& out);

    protected:
        friend class DRef;
        Data* GetDataInstance(int type);
        void  DeleteDataInstance(Data* pData);

    private:
        TypeManager m_typeManager;
};


inline int DataManager::ConvertUuid(const QString& uuid)
{
    return m_typeManager.ConvertUuid(uuid);
}


inline QString DataManager::GetUuid(int type)
{
    return m_typeManager.TypeRegistered(type) ?
           m_typeManager.GetUuid(type) :
           QString();
}


inline void DataManager::DeleteDataInstance(Data* pData)
{
    m_typeManager.DeleteInstance(pData);
}


} // namespace BOI


#endif //__BOI_DATAMANAGER_H

