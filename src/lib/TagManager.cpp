/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QStringBuilder>
#include <QSqlQuery>
#include <QVariant>
#include "TagManager.h"


namespace BOI {


TagManager::TagManager()
    : m_tagFilePath(),
      m_database()
{
}


TagManager::~TagManager()
{
    if (m_database.isOpen())
    {
        m_database.close();
    }
}


bool TagManager::Initialize(const QString& tagFilePath)
{
    m_tagFilePath = tagFilePath;

    m_database = QSqlDatabase::addDatabase("QSQLITE", "TagsDB");
    m_database.setDatabaseName(m_tagFilePath);

    if (!m_database.open())
    {
        return false;
    }

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    QString queryString = "CREATE TABLE IF NOT EXISTS Tags"
                          "("
                              "tag  VARCHAR(30),"
                              "uuid VARCHAR(50),"
                              "type INTEGER,"
                              "PRIMARY KEY(tag, uuid)"
                          ")";

    return query.exec(queryString);
}


bool TagManager::AddTag(const QString& tag,
                        const QString& uuid,
                        UuidType type)
{
    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    QString queryString = "INSERT INTO Tags VALUES('" % tag % "','" %
                          uuid % "'," % QString::number((int)type) % ")";

    return query.exec(queryString);
}


QList<TypedUuid> TagManager::GetUuids(const QString& tag, UuidType types)
{
    QList<TypedUuid> result;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    QString queryString = "SELECT uuid, type FROM Tags WHERE tag='" % tag % "'";

    if (query.exec(queryString))
    {
        while (query.next())
        {
            TypedUuid uuid;
            uuid.item2 = (UuidType)query.value(1).toInt();

            if (uuid.item2 & types)
            {
                uuid.item1 = query.value(0).toString();
                result.append(uuid);
            }
        }
    }

    return result;
}


} // namespace BOI

