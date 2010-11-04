/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_PLUGINMANAGER_H
#define __BOI_PLUGINMANAGER_H


#include <QHash>
#include <QString>


namespace BOI {


class Plugin;
class MenuManager;
class DataManager;
class ActionManager;
class ComponentManager;
class DescriptionManager;


class PluginManager
{
    public:
        PluginManager(DescriptionManager* pDescriptionManager,
                      ComponentManager*   pComponentManager,
                      ActionManager*      pActionManager,
                      DataManager*        pDataManager,
                      MenuManager*        pMenuManager);

        ~PluginManager();

        void LoadPlugins(const QString& path);

    protected:
        typedef struct PluginInfo
        {
            QString uuid;
            QString version;
            QString fileName;
            QString title;
            QString description;
            Plugin* pInstance;
        } PluginInfo;

    protected:
        void LoadPlugin(const QString& path,
                        const QString& infoFileName);

        bool IsValid(PluginInfo* pPluginInfo);

    private:
        MenuManager*        m_pMenuManager;
        DataManager*        m_pDataManager;
        ActionManager*      m_pActionManager;
        ComponentManager*   m_pComponentManager;
        DescriptionManager* m_pDescriptionManager;

        QHash<QString, PluginInfo*> m_plugins;
};


} // namespace BOI


#endif //__BOI_PLUGINMANAGER_H

