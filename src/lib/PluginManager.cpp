/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QDir>
#include <QFile>
#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QTextStream>
#include <QStringList>
#include <QPluginLoader>
#include "Plugin.h"
#include "Description.h"
#include "MenuManager.h"
#include "DataManager.h"
#include "ActionManager.h"
#include "ComponentManager.h"
#include "DescriptionManager.h"
#include "PluginManager.h"


namespace BOI {


PluginManager::PluginManager(DescriptionManager* pDescriptionManager,
                             ComponentManager*   pComponentManager,
                             ActionManager*      pActionManager,
                             DataManager*        pDataManager,
                             MenuManager*        pMenuManager)
    : m_pMenuManager(pMenuManager),
      m_pDataManager(pDataManager),
      m_pActionManager(pActionManager),
      m_pComponentManager(pComponentManager),
      m_pDescriptionManager(pDescriptionManager),
      m_plugins()
{
}


PluginManager::~PluginManager()
{
    QHashIterator<QString, PluginInfo*> it(m_plugins);
    while (it.hasNext())
    {
        it.next();
        delete it.value();
    }
}


void PluginManager::LoadPlugins(const QString& path)
{
    QDir pluginsDir(path);
    pluginsDir.setFilter(QDir::Dirs |
                         QDir::Files |
                         QDir::NoDotAndDotDot);

    foreach (QString fileName, pluginsDir.entryList())
    {
        QFileInfo fileInfo(pluginsDir.absoluteFilePath(fileName));

        if (fileInfo.isDir())
        {
            LoadPlugins(fileName);
        }
        else if (fileInfo.isFile())
        {
            if (fileInfo.suffix().toLower() == "info")
            {
                LoadPlugin(fileInfo.absolutePath(),
                           fileInfo.fileName());
            }
        }
    }
}


void PluginManager::LoadPlugin(const QString& path,
                               const QString& infoFileName)
{
    PluginInfo* pPluginInfo = new PluginInfo;

    QString fullInfoPath = path + "/" + infoFileName;

    QFile infoFile(fullInfoPath);
    if (infoFile.open(QFile::ReadOnly))
    {
        QTextStream in(&infoFile);

        QString line = in.readLine();
        while (!line.isNull())
        {
            line = line.trimmed();

            if (!line.isEmpty() && !line.startsWith("#"))
            {
                QStringList parts = line.split("=");

                if (parts.size() == 2)
                {
                    QString key = parts.at(0).trimmed().toLower();
                    QString value = parts.at(1).trimmed();

                    if (key == "uuid")
                    {
                        pPluginInfo->uuid = value.toLower();
                    }
                    else if (key == "version")
                    {
                        pPluginInfo->version = value;
                    }
                    else if (key == "filename")
                    {
                        pPluginInfo->fileName = value;
                    }
                    else if (key == "title")
                    {
                        pPluginInfo->title = value;
                    }
                    else if (key == "description")
                    {
                        pPluginInfo->description = value;
                    }
                }
            }

            line = in.readLine();
        }
    }

    if (!IsValid(pPluginInfo))
    {
        delete pPluginInfo;
    }
    else
    {
#if defined(Q_OS_LINUX)
        QString fullPluginPath = path + "/lib" + pPluginInfo->fileName + ".so";
#elif defined(Q_OS_WIN32)
        QString fullPluginPath = path + "/" + pPluginInfo->fileName + ".dll";
#endif

        QPluginLoader pluginLoader(fullPluginPath);
        QObject* pObject = pluginLoader.instance();

        if (pObject)
        {
            Plugin* pPlugin = qobject_cast<Plugin*>(pObject);

            if (pPlugin)
            {
                Description* pDescription = new Description;
                pDescription->SetBody(pPluginInfo->description);
                pDescription->SetUuidMajor(pPluginInfo->uuid);
                pDescription->SetTitle(pPluginInfo->title);

                m_pDescriptionManager->RegisterDescription(pDescription);

                pPlugin->RegisterMenus(m_pMenuManager);
                pPlugin->RegisterDataFactories(m_pDataManager);
                pPlugin->RegisterActionFactories(m_pActionManager);
                pPlugin->RegisterDescriptions(m_pDescriptionManager);
                pPlugin->RegisterComponentFactories(m_pComponentManager);

                pPluginInfo->pInstance = pPlugin;
                m_plugins.insert(pPluginInfo->uuid, pPluginInfo);
            }
        }
    }
}


bool PluginManager::IsValid(PluginInfo* pPluginInfo)
{
    bool isValid = false;

    if ((!pPluginInfo->uuid.isEmpty()) &&
        (!pPluginInfo->version.isEmpty()) &&
        (!pPluginInfo->fileName.isEmpty()))
    {
        isValid = true;
    }

    return isValid;
}


} // namespace BOI

