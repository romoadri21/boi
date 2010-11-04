/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CONNECTIONS_H
#define __BOI_CONNECTIONS_H


#include "Interface.h"
#include "Mutex.h"
#include "CRef.h"
#include "DRef.h"


namespace BOI {


class ISI;
class Component;


class Connections
{
    public:
        Connections();

        void Initialize(ISI* pISI, Interface* pInterface, int componentId);
        void Destroy();

        void AddEmitterRecipient(int emitter, CRef cref, ReceiverFunc func);
        void RemoveEmitterRecipient(int emitter, CRef cref);

        bool EmitterHasNew(int emitter);
        bool EmitterConnected(int emitter);

        void Emit(int emitter, DRef& data, bool release, bool newOnly);
        bool EmitTo(int emitter, DRef& data, int componentId, bool release);

        bool SetCallerTarget(int caller,
                             CRef cref,
                             int funcSet,
                             int numFuncs,
                             const FuncSetFunc* pFuncs);

        bool ClearCallerTarget(int caller);

        bool StartTransaction(int caller);
        void EndTransaction(int caller);
        DRef Call(int caller, int func, DRef& dref, bool passThru);

    protected:
        typedef struct
        {
            CRef cref;
            Component* pComponent;

            int  id;
            bool inTransaction;

            int funcSet;
            int numFuncs;
            const FuncSetFunc* pFuncs;
        } CallerTarget;

        typedef struct EmitterRecipient
        {
            CRef cref;
            bool isNew;
            ReceiverFunc func;
            EmitterRecipient* pNext;
        } EmitterRecipient;

        typedef struct
        {
            int numNewRecipients;
            int numEmitterRecipients;
            EmitterRecipient* pHead;
        } EmitterRecipientsHead;

    private:
        int m_numCallers;
        int m_numEmitters;

        Mutex m_callerTargetsLock;
        CallerTarget* m_callerTargets;

        Mutex m_emitterRecipientsLock;
        EmitterRecipientsHead* m_emitterRecipients;

        int m_componentId;

        ISI* m_pISI;
};


} // namespace BOI


#endif //__BOI_CONNECTIONS_H

