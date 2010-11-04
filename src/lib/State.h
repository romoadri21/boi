/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STATE_H
#define __BOI_STATE_H


#include <QVariant>
#include "CRef.h"
#include "CRefList.h"
#include "StateId.h"
#include "Events/ResizeEventListener.h"


class QString;
class QReadWriteLock;


namespace BOI {


class ISI;
class DRef;


/*
 * The system wide data representing
 * the state of the instance.
 */

class State
    : public ResizeEventListener
{
    public:
        State();
        ~State();

        void SetISI(ISI* pISI);

        bool Load(const QString& path);
        bool Save(const QString& path);

        bool Get(StateId stateId, QVariant& value);
        bool Set(StateId stateId, const QVariant& value);

        void RegisterListener(StateId stateId, CRef cref, QVariant& value);

        void UnregisterListener(StateId stateId, CRef cref);
        void UnregisterAllListeners();

        void HandleResizeEvent(ResizeEvent* pEvent);

        /*
         * Methods to access the state variables
         * that are not stored as QVariants.
         */

        CRef ActiveComponent();
        void SetActiveComponent(CRef cref);

        CRef KeyEventHandler();
        void SetKeyEventHandler(CRef cref);

        CRefList Selection();
        void SetSelection(CRefList crefList);

        /*
         * Resets the CRefs for the state variables
         * that are not stored as QVariants.
         */
        void ResetCRefs();

    protected:
        void SetData(StateId stateId, const QVariant& value);
        void UpdateViewData(StateId stateId, const QVariant& value);

        void PostToListeners(StateId stateId, DRef& dref);

    private:
        typedef struct Listener
        {
            CRef cref;
            Listener* pNext;
        } Listener;

        typedef struct
        {
            QVariant value;
            QReadWriteLock* pLock;
            Listener* pListeners;
        } StateData;

    private:
        StateData m_data[NumStateIds];

        ISI* m_pISI;

        /*
         * The state variables that are
         * not stored as QVariants.
         */

        CRef m_activeComponent;
        CRef m_keyEventHandler;

        CRefList m_selection;
};


} // namespace BOI


#endif //__BOI_STATE_H

