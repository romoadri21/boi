/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "ISI.h"
#include "GuiRequest.h"
#include "ComponentData.h"
#include "Connections.h"
#include "StandardEmitters.h"
#include "StandardDataTypes.h"
#include "Component.h"


namespace BOI {


Component::Component(int type)
    : Object(type)
{
    m_pData = new ComponentData(this);
}


Component::~Component()
{
    delete m_pData;
}


bool Component::ConnectToFuncSet(int caller, CRef cref, int funcSet)
{
    bool connected = false;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        Interface* pInterface = pComponent->IFace();

        const FuncSetFunc* pFuncs = pInterface->GetFuncSetFuncs(funcSet);
        if (pFuncs != NULL)
        {
            int numFuncs = pInterface->NumFuncSetFuncs(funcSet);
            connected = m_pData->connections.SetCallerTarget(caller, cref, funcSet, numFuncs, pFuncs);
        }

        cref.ReleaseInstance();
    }

    return connected;
}


bool Component::ConnectToReceiver(int emitter, CRef cref, int receiver)
{
    bool connected = false;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        ReceiverFunc func = pComponent->IFace()->GetReceiverFunc(receiver);
        if (func != NULL)
        {
            m_pData->connections.AddEmitterRecipient(emitter, cref, func);
            m_pData->pISI->PostEmitterConnectedTask(m_pData->cref, emitter, cref.Id());

            connected = true;
        }

        cref.ReleaseInstance();
    }

    return connected;
}


bool Component::DisconnectFromFuncSet(int caller)
{
    return m_pData->connections.ClearCallerTarget(caller);
}


void Component::DisconnectFromReceiver(int emitter, CRef cref)
{
    m_pData->connections.RemoveEmitterRecipient(emitter, cref);
}


bool Component::Initialize()
{
    return true;
}


void Component::Destroy()
{
}


void Component::Draw(QPainter* pPainter,
                     const QStyleOptionGraphicsItem* pOption)
{
    Q_UNUSED(pPainter);
    Q_UNUSED(pOption);
}


bool Component::HandlesTouchEvents()
{
    return false;
}


bool Component::HandlesKeyEvents()
{
    return false;
}


void Component::HandleTouchEvent(TouchEvent* pEvent)
{
    Q_UNUSED(pEvent);
}


void Component::HandleKeyEvent(KeyEvent* pEvent)
{
    Q_UNUSED(pEvent);
}


void Component::HandleStateChanged(StateId stateId, DRef& dref)
{
    Q_UNUSED(stateId);
    Q_UNUSED(dref);
}


void Component::HandleEmitterConnected(int emitter,
                                       int componentId)
{
    Q_UNUSED(emitter);
    Q_UNUSED(componentId);
}


int Component::AcceptTransaction(int funcSet)
{
    Q_UNUSED(funcSet);
    return -1;
}


void Component::TransactionEnded(int id)
{
    Q_UNUSED(id);
}


void Component::Import(const QHash<int, DRef>& in)
{
    Q_UNUSED(in);
}


void Component::Export(QHash<int, DRef>& out)
{
    Q_UNUSED(out);
}


CSI* Component::SI()
{
    return m_pData->pCSI;
}


void Component::LockDraw()
{
    m_pData->drawLock.Lock();
}


void Component::UnlockDraw()
{
    m_pData->drawLock.Unlock();
}


void Component::SetTransformOrigin(const QPointF& point)
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.data.point.x = point.x();
    request.data.point.y = point.y();
    request.type = GuiRequest::RequestType_SetTransformOrigin;

    m_pData->pISI->PostRequest(&request);
}


void Component::SetBoundingRect(const QRectF& rect)
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.data.rect.x = rect.x();
    request.data.rect.y = rect.y();
    request.data.rect.width  = rect.width();
    request.data.rect.height = rect.height();
    request.type = GuiRequest::RequestType_SetBoundingRect;

    m_pData->pISI->PostRequest(&request);
}


void Component::SetPosition(const QPointF& pos)
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.data.point.x = pos.x();
    request.data.point.y = pos.y();
    request.type = GuiRequest::RequestType_SetPosition;

    m_pData->pISI->PostRequest(&request);
}


void Component::SetRotation(qreal rotation)
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.data.rotation = rotation;
    request.type = GuiRequest::RequestType_SetRotation;

    m_pData->pISI->PostRequest(&request);
}


void Component::SetOpacity(qreal opacity)
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.data.opacity = opacity;
    request.type = GuiRequest::RequestType_SetOpacity;

    m_pData->pISI->PostRequest(&request);
}


void Component::SetVisible(bool visible)
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.data.boolean = visible;
    request.type = GuiRequest::RequestType_SetVisible;

    m_pData->pISI->PostRequest(&request);
}


void Component::Update()
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.type = GuiRequest::RequestType_Update;

    m_pData->pISI->PostRequest(&request);
}


void Component::Update(const QRectF& rect)
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.data.rect.x = rect.x();
    request.data.rect.y = rect.y();
    request.data.rect.width  = rect.width();
    request.data.rect.height = rect.height();
    request.type = GuiRequest::RequestType_UpdateRect;

    m_pData->pISI->PostRequest(&request);
}


bool Component::EmitterHasNew(int emitter)
{
    return m_pData->connections.EmitterHasNew(emitter);
}


bool Component::EmitterConnected(int emitter)
{
    return m_pData->connections.EmitterConnected(emitter);
}


void Component::Emit(int emitter, DRef& data, bool release)
{
    /*
     * Components can only Emit from non-standard emitters.
     * The system handles everything for standard emitters.
     */
    if (emitter < StandardEmitters::NumEmitters) return;

    m_pData->connections.Emit(emitter, data, release, false);
}


bool Component::EmitTo(int emitter, DRef& data, int componentId, bool release)
{
    /*
     * Components can only Emit from non-standard emitters.
     * The system handles everything for standard emitters.
     */
    if (emitter < StandardEmitters::NumEmitters) return false;

    return m_pData->connections.EmitTo(emitter, data, componentId, release);
}


void Component::EmitToNew(int emitter, DRef& data, bool release)
{
    /*
     * Components can only Emit from non-standard emitters.
     * The system handles everything for standard emitters.
     */
    if (emitter < StandardEmitters::NumEmitters) return;

    m_pData->connections.Emit(emitter, data, release, true);
}


bool Component::StartTransaction(int caller)
{
    return m_pData->connections.StartTransaction(caller);
}


void Component::EndTransaction(int caller)
{
    m_pData->connections.EndTransaction(caller);
}


DRef Component::Call(int caller, int func, DRef& dref, bool passThru)
{
    return m_pData->connections.Call(caller, func, dref, passThru);
}


void Component::HandleEmitterConnectedInternal(int emitter,
                                               int componentId)
{
    if (emitter >= StandardEmitters::NumEmitters)
    {
        /*
         * Components should only receive this notification for
         * emitters that they have defined themselves. The system
         * handles the notifications for the standard emitters.
         */
        HandleEmitterConnected(emitter, componentId);
    }
    else if (emitter == BOI_STD_E(ParentBoundingBox))
    {
        GuiRequest request;

        request.cref = m_pData->cref;
        request.data.emitData.emitter = emitter;
        request.data.emitData.newOnly = true;
        request.type = GuiRequest::RequestType_Emit;

        m_pData->pISI->PostRequest(&request);
    }
}


DRef Component::CallFunc(int funcSet, int func, DRef& dref, bool passThru)
{
    DRef result;

    Interface* pInterface = IFace();

    const FuncSetFunc* pFuncs = pInterface->GetFuncSetFuncs(funcSet);
    if (pFuncs != NULL)
    {
        int numFuncs = pInterface->NumFuncSetFuncs(funcSet);

        if ((func >= 0) && (func < numFuncs))
        {
            int transactionId = AcceptTransaction(funcSet);
            if (transactionId != -1)
            {
                FuncSetFunc funcSetFunc = pFuncs[func];

                if (passThru)
                {
                    result = funcSetFunc(this, transactionId, dref);
                }
                else
                {
                    DRef drefCopy = dref;
                    result = funcSetFunc(this, transactionId, drefCopy);
                }

                TransactionEnded(transactionId);
            }
        }
    }

    return result;
}


void Component::PostToReceiver(int receiver, DRef& data, bool release)
{
    ReceiverFunc func = IFace()->GetReceiverFunc(receiver);
    if (func != NULL)
    {
        m_pData->pISI->PostEmitTask(-1, m_pData->cref, func, data, release);
    }
}


void Component::SetParent(CRef& parent)
{
    GuiRequest request;

    request.cref = m_pData->cref;
    request.cref2 = parent;
    request.type = GuiRequest::RequestType_SetParent;

    m_pData->pISI->PostRequest(&request);
}


void Component::SetPosition(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Point))
    {
        SetPosition(*dref.GetReadInstance<QPointF>());
    }
}


void Component::SetRotation(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Float))
    {
        SetRotation(*dref.GetReadInstance<float>());
    }
    else if (dref.Type() == BOI_STD_D(Double))
    {
        SetRotation(*dref.GetReadInstance<double>());
    }
    else if (dref.Type() == BOI_STD_D(Int))
    {
        SetRotation(*dref.GetReadInstance<int>());
    }
}


void Component::SetOpacity(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Float))
    {
        SetOpacity(*dref.GetReadInstance<float>());
    }
    else if (dref.Type() == BOI_STD_D(Double))
    {
        SetOpacity(*dref.GetReadInstance<double>());
    }
}


void Component::SetVisible(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Bool))
    {
        SetVisible(*dref.GetReadInstance<bool>());
    }
    else if (dref.Type() == BOI_STD_D(Int))
    {
        SetVisible(*dref.GetReadInstance<int>());
    }
}


} // namespace BOI

