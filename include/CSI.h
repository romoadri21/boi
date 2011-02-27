/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CSI_H
#define __BOI_CSI_H


#include "BOILib.h"
#include "StateId.h"
#include "DRef.h"


class QString;
class QVariant;
class QDataStream;


namespace BOI {


class CustomEventsFactory;
class ComponentManager;
class EventDispatcher;
class ActionManager;
class DataManager;
class ActionArgs;
class Component;
class MutexBase;
class Mutex;
class State;


/*
 * Component System Interface: the inteface
 * to the system for components.
 */

class BOI_LIB_EXPORT CSI
{
    public:
        CSI(CustomEventsFactory* pCustomEventsFactory,
            ComponentManager*    pComponentManager,
            EventDispatcher*     pEventDispatcher,
            ActionManager*       pActionManager,
            DataManager*         pDataManager,
            MutexBase*           pMutexBase,
            State*               pState);

        void RegisterMutex(Mutex* pMutex);

        int ConvertUuid_A(const QString& uuid);
        int ConvertUuid_C(const QString& uuid);
        int ConvertUuid_D(const QString& uuid);

        DRef NewData(int type);

        DRef ImportData(QDataStream& in);
        void ExportData(DRef& dref, QDataStream& out);

        bool GetState(StateId stateId, QVariant& value);
        bool SetState(StateId stateId, const QVariant& value);

        void RegisterStateListener(StateId stateId, Component* pComponent, QVariant& value);
        void UnregisterStateListener(StateId stateId, Component* pComponent);

        void SetActiveAction(int action, const ActionArgs* pArgs=NULL);
        void UpdateActiveAction(int action, const ActionArgs* pArgs=NULL);

        void PostKeyPressEvent(int defaultKey, int shiftKey);
        void PostKeyReleaseEvent(int defaultKey, int shiftKey);

    protected:
        CustomEventsFactory* m_pCustomEventsFactory;
        ComponentManager*    m_pComponentManager;
        EventDispatcher*     m_pEventDispatcher;
        ActionManager*       m_pActionManager;
        DataManager*         m_pDataManager;
        MutexBase*           m_pMutexBase;
        State*               m_pState;
};


} // namespace BOI


#endif //__BOI_CSI_H

