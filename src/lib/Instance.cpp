/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QDir>
#include <QThread>
#include <QCoreApplication>
#include "Base.h"
#include "Instance.h"


/*
 * The minimum number of task threads to use if a
 * value is not specified during initialization.
 */
#define BOI_MIN_TASK_THREADS 2
#define BOI_DEFAULT_PLUGINS_DIRNAME "plugins"


namespace BOI {


Instance::Instance()
{
    m_pBase = new Base();
}


Instance::~Instance()
{
    delete m_pBase;
}


bool Instance::Initialize(QCoreApplication* pApp,
                          QWidget* pParent,
                          int numTaskThreads,
                          const QString& rootDirString,
                          const QString& pluginsDirString)
{
    if (numTaskThreads < 1)
    {
        /*
         * Note, idealThreadCount() returns the number of processor
         * cores, both real and logical, in the system.
         */
        numTaskThreads = QThread::idealThreadCount();
        if (numTaskThreads < BOI_MIN_TASK_THREADS)
        {
            numTaskThreads = BOI_MIN_TASK_THREADS;
        }
    }


    QDir rootDir(rootDirString);
    if (rootDirString.isEmpty())
    {
        /*
         * TODO: set this to something more appropriate?
         * The users home directory + .BOI? Or get it
         * from an environmental variable or both?
         */
        rootDir.setPath(pApp->applicationDirPath());
    }

    if (!rootDir.exists())
    {
        /*
         * If the directory doesn't exist,
         * try to create the directory.
         */
        QString path = rootDir.absolutePath();
        if (!rootDir.mkpath(path))
        {
            return false;
        }
    }

    QDir pluginsDir(pluginsDirString);
    if (pluginsDirString.isEmpty())
    {
        QString path = rootDir.absolutePath()
                       + '/'
                       + BOI_DEFAULT_PLUGINS_DIRNAME;

        pluginsDir.setPath(path);
    }

    if (!pluginsDir.exists())
    {
        pluginsDir = rootDir;
    }


    return m_pBase->Initialize(pApp,
                               pParent,
                               numTaskThreads,
                               rootDir.absolutePath(),
                               pluginsDir.absolutePath());
}


void Instance::Start()
{
    m_pBase->Start();
}


} // namespace BOI

