/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COMPONENT_H
#define __BOI_COMPONENT_H


#include <QPointF>
#include <QRectF>
#include <QHash>
#include "CRef.h"
#include "DRef.h"
#include "BOILib.h"
#include "Object.h"
#include "StateId.h"
#include "Interface.h"
#include "ProfileType.h"


class QString;
class QPainter;
class QStyleOptionGraphicsItem;


namespace BOI {


class CSI;
class KeyEvent;
class TouchEvent;
class ComponentData;


class BOI_LIB_EXPORT Component
    : public Object
{
    public:
        Component(int type);

        /* 
         * A virtual destructor is required so that the
         * destructor in derived classes gets called.
         */
        virtual ~Component();

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

        virtual bool HandlesTouchEvents();
        virtual bool HandlesKeyEvents();

        virtual void HandleTouchEvent(TouchEvent* pEvent);
        virtual void HandleKeyEvent(KeyEvent* pEvent);

        virtual void HandleStateChanged(StateId stateId, DRef& dref);

        virtual void HandleEmitterConnected(int emitter,
                                            int componentId);

        virtual int AcceptTransaction(int funcSet);
        virtual void TransactionEnded(int id);

        virtual void Import(const QHash<int, DRef>& in);
        virtual void Export(QHash<int, DRef>& out);

        virtual Interface* IFace() = 0;

        CSI* SI();

        void LockDraw();
        void UnlockDraw();

        void SetTransformOrigin(const QPointF& point);
        void SetBoundingRect(const QRectF& rect);
        void SetPosition(const QPointF& pos);
        void SetRotation(qreal rotation);
        void SetOpacity(qreal opacity);
        void SetVisible(bool visible);

        void Update();
        void Update(const QRectF& rect);

        bool EmitterHasNew(int emitter);
        bool EmitterConnected(int emitter);

        void Emit(int emitter, DRef& data, bool release=false);
        bool EmitTo(int emitter, DRef& data, int componentId, bool release=false);
        void EmitToNew(int emitter, DRef& data, bool release=false);

        bool StartTransaction(int caller);
        void EndTransaction(int caller);
        DRef Call(int caller, int func, DRef& dref, bool passThru=false);

        BOI_DEFINE_RECEIVER_FUNC(Component, SetPosition);
        BOI_DEFINE_RECEIVER_FUNC(Component, SetRotation);
        BOI_DEFINE_RECEIVER_FUNC(Component, SetOpacity);
        BOI_DEFINE_RECEIVER_FUNC(Component, SetVisible);

    private:
        /*
         * Force the use of the Component(int type) constructor
         * since all Component objects should have an assigned
         * type.
         */
        Component();

        const QStringList GetProfile(ProfileType type=ProfileType_Full);

        int GetCaller(const QString& uuid);
        int GetFuncSet(const QString& uuid);
        int GetEmitter(const QString& uuid);
        int GetReceiver(const QString& uuid);

        bool ConnectToFuncSet(int caller, CRef cref, int funcSet);
        bool ConnectToReceiver(int emitter, CRef cref, int receiver);

        bool DisconnectFromFuncSet(int caller);
        void DisconnectFromReceiver(int emitter, CRef cref);

        void HandleEmitterConnectedInternal(int emitter, int componentId);

        DRef CallFunc(int funcSet, int func, DRef& dref, bool passThru=false);

        void PostToReceiver(int receiver, DRef& data, bool release=false);

    private:
        ComponentData* m_pData;

        friend class CSI;
        friend class ASI;
        friend class Connections;
        friend class GraphicsItem;
        friend class TaskProcessor;
        friend class ComponentManager;
        friend class InputEventHandler;
        friend class GuiRequestHandler;
        friend class CollectionManager;
};


inline const QStringList Component::GetProfile(ProfileType type)
{
    return IFace()->GetProfile(type);
}


inline int Component::GetCaller(const QString& uuid)
{
    return IFace()->GetCaller(uuid);
}


inline int Component::GetFuncSet(const QString& uuid)
{
    return IFace()->GetFuncSet(uuid);
}


inline int Component::GetEmitter(const QString& uuid)
{
    return IFace()->GetEmitter(uuid);
}


inline int Component::GetReceiver(const QString& uuid)
{
    return IFace()->GetReceiver(uuid);
}


} // namespace BOI


#endif //__BOI_COMPONENT_H

