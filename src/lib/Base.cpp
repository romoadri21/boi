/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QCoreApplication>
#include "View.h"
#include "Scene.h"
#include "State.h"
#include "Factories.h"
#include "MutexBase.h"
#include "TagManager.h"
#include "DataManager.h"
#include "MenuManager.h"
#include "ActionEngine.h"
#include "PluginManager.h"
#include "ActionManager.h"
#include "TaskProcessor.h"
#include "TransformManager.h"
#include "ComponentManager.h"
#include "InputEventHandler.h"
#include "GuiRequestHandler.h"
#include "CollectionManager.h"
#include "DescriptionManager.h"
#include "Events/EventDispatcher.h"
#include "Factories/CustomEventsFactory.h"
#include "CRefListPool.h"
#include "ASI.h"
#include "ISI.h"
#include "Base.h"


namespace BOI {


Base::Base()
    : QObject(),
      m_pView(NULL),
      m_pScene(NULL),
      m_pState(NULL),
      m_pFactories(NULL),
      m_pMutexBase(NULL),
      m_pTagManager(NULL),
      m_pDataManager(NULL),
      m_pMenuManager(NULL),
      m_pActionEngine(NULL),
      m_pCRefListPool(NULL),
      m_pPluginManager(NULL),
      m_pActionManager(NULL),
      m_pTaskProcessor(NULL),
      m_pEventDispatcher(NULL),
      m_pTransformManager(NULL),
      m_pComponentManager(NULL),
      m_pInputEventHandler(NULL),
      m_pGuiRequestHandler(NULL),
      m_pCollectionManager(NULL),
      m_pDescriptionManager(NULL),
      m_pCustomEventsFactory(NULL),
      m_pASI(NULL),
      m_pCSI(NULL),
      m_pISI(NULL),
      m_stateFilePath()
{
}


Base::~Base()
{
    delete m_pCustomEventsFactory;
    delete m_pDescriptionManager;
    delete m_pCollectionManager;
    delete m_pGuiRequestHandler;
    delete m_pInputEventHandler;
    delete m_pComponentManager;
    delete m_pTransformManager;
    delete m_pEventDispatcher;
    delete m_pTaskProcessor;
    delete m_pActionManager;
    delete m_pPluginManager;
    delete m_pActionEngine;
    delete m_pMenuManager;
    delete m_pDataManager;
    delete m_pTagManager;
    delete m_pFactories;
    delete m_pState;
    delete m_pASI;
    delete m_pCSI;
    delete m_pISI;
    delete m_pScene;
    delete m_pView;

    delete m_pMutexBase;

    /*
     * All CRefLists should have
     * already been deleted.
     */
    delete m_pCRefListPool;
}


bool Base::Initialize(QCoreApplication* pApp,
                      QWidget* pParent,
                      int numTaskThreads,
                      const QString& rootDir,
                      const QString& pluginsDir)
{
    try
    {
        m_pMutexBase = new MutexBase();

        m_pState = new State();
        m_stateFilePath = rootDir + '/' + BOI_STATE_FILE_NAME;
        m_pState->Load(m_stateFilePath);

        QString tagFilePath = rootDir + '/' + BOI_TAG_FILE_NAME;
        m_pTagManager = new TagManager();
        if (!m_pTagManager->Initialize(tagFilePath)) throw 0;

        QString collectionsPath = rootDir + '/' + BOI_COLLECTIONS_DIR_NAME;
        m_pCollectionManager = new CollectionManager();
        if (!m_pCollectionManager->Initialize(collectionsPath)) throw 0;

        m_pTaskProcessor = new TaskProcessor();
        if (!m_pTaskProcessor->Initialize(numTaskThreads)) throw 0;

        m_pMenuManager = new MenuManager();
        m_pMenuManager->InitStandardMenus();
        m_pMenuManager->InitStandardProfiles();

        m_pDataManager = new DataManager();
        if (!m_pDataManager->Initialize()) throw 0;

        m_pActionManager = new ActionManager();
        if (!m_pActionManager->Initialize()) throw 0;

        m_pComponentManager = new ComponentManager();
        if (!m_pComponentManager->Initialize()) throw 0;

        m_pFactories = new Factories();
        m_pFactories->InitDataFactories(m_pDataManager);
        m_pFactories->InitActionFactories(m_pActionManager);
        m_pFactories->InitComponentFactories(m_pComponentManager);

        m_pCustomEventsFactory = new CustomEventsFactory();

        m_pDescriptionManager = new DescriptionManager();
        m_pDescriptionManager->InitStandardDescriptions();

        m_pActionEngine = new ActionEngine();
        m_pActionEngine->SetActionManager(m_pActionManager);

        m_pInputEventHandler = new InputEventHandler(m_pActionEngine);

        m_pEventDispatcher = new EventDispatcher();

        m_pScene = new Scene();

        m_pView = new View(m_pScene, pParent);
        m_pView->SetEventDispatcher(m_pEventDispatcher);
        m_pView->setWindowTitle(BOI_WINDOW_TITLE);
        m_pView->resize(1000, 700);

        m_pTransformManager = new TransformManager(m_pView, m_pActionEngine);

        m_pGuiRequestHandler = new GuiRequestHandler();
        m_pGuiRequestHandler->SetScene(m_pScene);
        m_pGuiRequestHandler->SetEventReceiver(m_pEventDispatcher);
        m_pGuiRequestHandler->SetCustomEventsFactory(m_pCustomEventsFactory);

        m_pEventDispatcher->SetCloseEventListener(this);
        m_pEventDispatcher->SetResizeEventListener(m_pState);
        m_pEventDispatcher->SetKeyEventListener(m_pInputEventHandler);
        m_pEventDispatcher->SetSetActionEventListener(m_pActionEngine);
        m_pEventDispatcher->SetTouchEventListener(m_pInputEventHandler);
        m_pEventDispatcher->SetUpdateActionEventListener(m_pActionEngine);
        m_pEventDispatcher->SetCustomEventsFactory(m_pCustomEventsFactory);
        m_pEventDispatcher->SetGuiRequestEventListener(m_pGuiRequestHandler);
        m_pEventDispatcher->SetInputModeChangeEventListener(m_pInputEventHandler);

        m_pCRefListPool = new CRefListPool();

        m_pISI = new ISI(m_pGuiRequestHandler,
                         m_pComponentManager,
                         m_pTaskProcessor,
                         m_pCRefListPool,
                         m_pDataManager,
                         m_pMutexBase);

        m_pCSI = new CSI(m_pCustomEventsFactory,
                         m_pComponentManager,
                         m_pEventDispatcher,
                         m_pActionManager,
                         m_pDataManager,
                         m_pMutexBase,
                         m_pState);

        m_pASI = new ASI(m_pCustomEventsFactory,
                         m_pDescriptionManager,
                         m_pCollectionManager,
                         m_pGuiRequestHandler,
                         m_pComponentManager,
                         m_pTransformManager,
                         m_pEventDispatcher,
                         m_pActionManager,
                         m_pCRefListPool,
                         m_pDataManager,
                         m_pMenuManager,
                         m_pTagManager,
                         m_pMutexBase,
                         m_pState,
                         m_pScene,
                         m_pView);

        m_pState->SetISI(m_pISI);

        m_pActionEngine->SetASI(m_pASI);

        m_pComponentManager->SetCSI(m_pCSI);
        m_pComponentManager->SetISI(m_pISI);

        m_pCollectionManager->SetISI(m_pISI);

        m_pInputEventHandler->SetASI(m_pASI);
        m_pInputEventHandler->SetISI(m_pISI);

        m_pPluginManager = new PluginManager(m_pDescriptionManager,
                                             m_pComponentManager,
                                             m_pActionManager,
                                             m_pDataManager,
                                             m_pMenuManager);

        m_pPluginManager->LoadPlugins(pluginsDir);

        connect(pApp, SIGNAL(aboutToQuit()),
                this, SLOT(HandleQuit()));

        return true;
    }
    catch (...) { }

    return false;
}


void Base::Start()
{
    m_pTaskProcessor->Start();

    m_pView->show();
    m_pView->CenterOn(0,0);
}


void Base::HandleCloseEvent()
{
}


void Base::HandleQuit()
{
    m_pInputEventHandler->Destroy();

    m_pActionEngine->Execute(BOI_AEOP(Clear));
    m_pActionManager->DestroyActions();

    m_pComponentManager->DestroyAll(true);

    /*
     * TaskProcessor::Destroy must be called after
     * ComponentManager::DestroyAll() because until
     * all the components have been destroyed, they
     * can still post tasks (ie. emit inside of
     * Component::Destroy).
     * 
     * Note, GuiRequestHandler::FlushRequests() will not
     * be able to emit any tasks since it is called after
     * ComponentManager::DestroyAll() and thus will not
     * be able to get any valid instances to components
     * from any CRefs.
     */
    m_pTaskProcessor->Destroy();

    m_pGuiRequestHandler->FlushRequests();

    /*
     * Reset all CRefs in State so that the
     * referenced components get deleted correctly.
     * Note, this should happen after TaskProcessor::Destroy() and
     * GuiRequestHandler::FlushRequests() since a component can
     * technically still register as a state listener from within
     * these calls.
     */
    m_pState->ResetCRefs();
    m_pState->UnregisterAllListeners();

    m_pState->Save(m_stateFilePath);

    m_pTransformManager->StopTransformers();
}


} // namespace BOI

