/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TAGMANAGER_H
#define __BOI_TAGMANAGER_H


#include <QList>
#include <QString>
#include <QSqlDatabase>
#include "UuidType.h"


namespace BOI {


class TagManager
{
    public:
        TagManager();
        ~TagManager();

        bool Initialize(const QString& tagFilePath);

        bool AddTag(const QString& tag,
                    const QString& uuid,
                    UuidType type);

        QList<TypedUuid> GetUuids(const QString& tag,
                                  UuidType types=UuidType_All);

    private:
        QString m_tagFilePath;
        QSqlDatabase m_database;
};


} // namespace BOI


#endif //__BOI_TAGMANAGER_H

