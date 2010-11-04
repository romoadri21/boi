/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MENUREGISTRAR_H
#define __BOI_MENUREGISTRAR_H


class QString;
class QStringList;


namespace BOI {


class MenuItem;


class MenuRegistrar
{
    public:
        virtual MenuItem* GetMenu(const QString& uuid) = 0;
        virtual bool RegisterMenu(const QString& uuid, MenuItem* pItem) = 0;
        virtual void RegisterProfile(const QStringList& uuids,
                                     const QStringList& menus) = 0;
};


} // namespace BOI


#endif //__BOI_MENUREGISTRAR_H

