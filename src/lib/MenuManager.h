/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MENUMANAGER_H
#define __BOI_MENUMANAGER_H


#include <QHash>
#include <QString>
#include <QStringList>
#include "MenuRegistrar.h"
#include "MenuSet.h"


namespace BOI {


class MenuItem;


class MenuManager
    : public MenuRegistrar
{
    public:
        MenuManager();
        ~MenuManager();

        void InitStandardMenus();
        void InitStandardProfiles();

        MenuItem* GetMenu(const QString& uuid);
        bool RegisterMenu(const QString& uuid, MenuItem* pItem);

        /*
         * Note, UUIDs are used instead of MenuItem*
         * to specify the menus since it is possible
         * that a plugin completely replaces the menu
         * thus invalidating any previously retrieved
         * root MenuItem*.
         * The uuids parameter should be a sorted list.
         */
        void RegisterProfile(const QStringList& uuids,
                             const QStringList& menus);

        /*
         * The profileId parameter is an integer that
         * uniquely identifies the profile parameter.
         * Internally, MenuManager uses the profileId
         * as a key for cached results. If the value
         * is -1 on input, the result is not cached.
         * If there are no matches for profile, then
         * the menuSet parameter is cleared.
         */
        void GetMenus(const QStringList& profile,
                      MenuSet& menuSet,
                      int profileId = -1);

        /*
         * Retrieve the cached result corresponding to
         * the profileId. If the cache does not contain
         * a value for this profileId, return false and
         * do nothing with the menuSet parameter.
         */
        bool GetMenus(int profileId, MenuSet& menuSet);

    private:
        typedef struct Profile
        {
            QStringList uuids;
            QStringList menus;
            Profile*    pNext;
            MenuSet     menuSet;
        } Profile;

    private:
        Profile* GetProfile(const QStringList& uuids,
                            bool* exactMatch);

        /*
         * Returns true if list1 contains list2.
         * Both lists should be ordered lists.
         * Both lists should not be empty.
         * All items before startIndex* are ignored.
         */
        bool Contains(const QStringList& list1,
                      const QStringList& list2,
                      int startIndex1 = 0,
                      int startIndex2 = 0);

        void FillMenuSet(MenuSet* pMenuSet, const QStringList& menuUuids);

    private:
        QHash<QString, MenuItem*> m_menus;
        QHash<QString, Profile*>  m_profiles;

        QHash<int, MenuSet*> m_cache;
};


} // namespace BOI


#endif //__BOI_MENUMANAGER_H

