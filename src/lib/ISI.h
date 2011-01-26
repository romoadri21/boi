/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ISI_H
#define __BOI_ISI_H


#include <QString>
#include "DRef.h"
#include "CRef.h"
#include "StateId.h"
#include "CRefList.h"
#include "Interface.h"
#include "ViewLayerId.h"


class QDataStream;


namespace BOI {


class GuiRequestHandler;
class ComponentManager;
class ComponentData;
class TaskProcessor;
class CRefListPool;
class DataManager;
class GuiRequest;
class TouchEvent;
class MutexBase;
class KeyEvent;
class Mutex;


/*
 * Internal System Interface: the internal
 * inteface to the system.
 */

class ISI
{
    public:
        ISI(GuiRequestHandler* pGuiRequestHandler,
            ComponentManager*  pComponentManager,
            TaskProcessor*     pTaskProcessor,
            CRefListPool*      pCRefListPool,
            DataManager*       pDataManager,
            MutexBase*         pMutexBase);

        CRef NewComponent(int type, ViewLayerId layer=ViewLayerId_Main);
        DRef NewData(int type);

        QString GetUuid_C(int type);
        QString GetUuid_D(int type);

        int ConvertUuid_C(const QString& uuid);
        int ConvertUuid_D(const QString& uuid);

        DRef ImportData(QDataStream& in);
        void ExportData(DRef& dref, QDataStream& out);

        void RegisterMutex(Mutex* pMutex);

        bool PostRequest(GuiRequest* pRequest);

        void ActivateComponent(ComponentData* pComponentData);

        void PostKeyEventTask(CRef cref, KeyEvent* pKeyEvent);
        void PostTouchEventTask(CRef cref, TouchEvent* pTouchEvent);
        void PostStateChangedTask(CRef cref, StateId stateId, DRef& dref);
        void PostEmitterConnectedTask(CRef cref, int emitter, int componentId);
        void PostEmitterDisconnectedTask(CRef cref, int emitter, int componentId);
        void PostEmitTask(int source, CRef cref, ReceiverFunc func, DRef& dref, bool release);

        void FreeTasks(ComponentData* pComponentData);

        CRefList NewCRefList();

    private:
        friend class CRef;
        void DestroyComponent(ComponentData* pComponentData);
        void DeleteComponent(ComponentData* pComponentData);

    private:
        GuiRequestHandler* m_pGuiRequestHandler;
        ComponentManager*  m_pComponentManager;
        TaskProcessor*     m_pTaskProcessor;
        CRefListPool*      m_pCRefListPool;
        DataManager*       m_pDataManager;
        MutexBase*         m_pMutexBase;
};


} // namespace BOI


#endif //__BOI_ISI_H

