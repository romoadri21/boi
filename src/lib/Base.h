/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_BASE_H
#define __BOI_BASE_H


#define BOI_WINDOW_TITLE         "BOI"
#define BOI_TAG_FILE_NAME        "tags.dat"
#define BOI_STATE_FILE_NAME      "state.dat"
#define BOI_COLLECTIONS_DIR_NAME "collections"


#include <QObject>
#include <QString>
#include "Events/CloseEventListener.h"


class QWidget;
class QCoreApplication;


namespace BOI {


class CustomEventsFactory;
class DescriptionManager;
class CollectionManager;
class GuiRequestHandler;
class InputEventHandler;
class ComponentManager;
class TransformManager;
class EventDispatcher;
class TaskProcessor;
class ActionManager;
class PluginManager;
class CRefListPool;
class ActionEngine;
class MenuManager;
class DataManager;
class TagManager;
class MutexBase;
class Factories;
class State;
class View;
class ASI;
class CSI;
class ISI;


class Base
    : public QObject,
      public CloseEventListener
{
    Q_OBJECT

    public:
        Base();
        ~Base();

        bool Initialize(QCoreApplication* pApp,
                        QWidget* pParent,
                        int numTaskThreads,
                        const QString& rootDir,
                        const QString& pluginsDir);

        void Start();

        void HandleCloseEvent();

    public slots:
        void HandleQuit();

    private:
        View*                m_pView;
        State*               m_pState;
        Factories*           m_pFactories;
        MutexBase*           m_pMutexBase;
        TagManager*          m_pTagManager;
        DataManager*         m_pDataManager;
        MenuManager*         m_pMenuManager;
        ActionEngine*        m_pActionEngine;
        CRefListPool*        m_pCRefListPool;
        PluginManager*       m_pPluginManager;
        ActionManager*       m_pActionManager;
        TaskProcessor*       m_pTaskProcessor;
        EventDispatcher*     m_pEventDispatcher;
        TransformManager*    m_pTransformManager;
        ComponentManager*    m_pComponentManager;
        InputEventHandler*   m_pInputEventHandler;
        GuiRequestHandler*   m_pGuiRequestHandler;
        CollectionManager*   m_pCollectionManager;
        DescriptionManager*  m_pDescriptionManager;
        CustomEventsFactory* m_pCustomEventsFactory;

        ASI* m_pASI;
        CSI* m_pCSI;
        ISI* m_pISI;

        QString m_stateFilePath;
};


} // namespace BOI


#endif //__BOI_BASE_H

