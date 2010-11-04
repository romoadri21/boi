/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include "Data.h"
#include "StandardDataTypes.h"
#include "DataManager.h"


namespace BOI {


DataManager::DataManager()
    : FactoryRegistrar(),
      m_typeManager()
{
}


bool DataManager::Initialize()
{
    /*
     * Reserve space and the matching system internal
     * type for the built-in DataTypes.
     */

    for (int i=0; i < StandardDataTypes::NumTypes; i++)
    {
        m_typeManager.RegisterType(NULL,
                                   NULL,
                                   NULL,
                                   StandardDataTypes::Uuid(i));
    }

    return true;
}


void DataManager::RegisterFactory(Factory* pFactory)
{
    pFactory->RegisterTypes(&m_typeManager);
}


DRef DataManager::GetData(int type)
{
    if (!m_typeManager.TypeRegistered(type))
    {
        /*
         * The caller is requesting a type that is not registered
         * with the system. Return an invalid DRef.
         */
        return DRef();
    }

    Data* pData = (Data*)m_typeManager.GetInstance(type);
    pData->m_pDataManager = this;
    return DRef(pData);
}


Data* DataManager::GetDataInstance(int type)
{
    Data* pData = (Data*)m_typeManager.GetInstance(type);
    pData->m_pDataManager = this;
    return pData;
}


DRef DataManager::Import(QDataStream& in)
{
    qint32 type;
    qint32 version;

    in >> version;
    in >> type;

    if (type == BOI_STD_D(Invalid))
    {
        return DRef();
    }

    // TODO: If the type is a recognized StandardDataType then
    // the uuid read should be skipped to avoid any extra
    // unnecessary processing (see QDataStream::skipRawData
    // and the QString section in "Serializing Qt Data Types").
    QString uuid;
    in >> uuid;

    QByteArray byteArray;
    in >> byteArray;

    if (version == BOI_STD_D_VERSION)
    {
        if (type >= StandardDataTypes::NumTypes)
        {
            type = ConvertUuid(uuid);
        }
    }
    else
    {
        /*
         * For unknown versions, see if the
         * uuid is supported by the system.
         */
        type = ConvertUuid(uuid);
    }

    if (type != -1)
    {
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::ReadOnly);

        QDataStream tempIn(&buffer);

        Data* pData = GetDataInstance(type);
        pData->Import(tempIn);

        buffer.close();

        return DRef(pData);
    }

    return DRef();
}


void DataManager::Export(DRef& dref, QDataStream& out)
{
    Data* pData = dref.m_pData;

    if (pData != NULL)
    {
        out << (qint32)BOI_STD_D_VERSION;
        out << (qint32)pData->Type();
        out << GetUuid(pData->Type());

        /*
         * Do not write the Data object directly to the
         * stream. This will allow implementations to
         * correctly skip over any unknown types. For
         * example, given the following stream:
         *
         *      out << QString << DRef << QString;
         * 
         * If the DRef data object was directly written
         * to the stream and then imported on a different
         * version of the system which does not support the
         * particular DRef type, the import would not be able
         * to read in the data and advance the stream to the
         * correct position to read the final QString.
         * Using a QByteArray avoids this situation because
         * the Import routine will always just read in a byte
         * array and thus advance the stream to the next
         * appropriate position (regardless of whether or not
         * the system supports the data type).
         */

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);

        QDataStream tempOut(&buffer);
        pData->Export(tempOut);

        buffer.close();

        out << byteArray;
    }
    else
    {
        out << (qint32)BOI_STD_D_VERSION;
        out << (qint32)BOI_STD_D(Invalid);
    }
}


} // namespace BOI

