/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ASI_H
#define __BOI_ASI_H


#include <QStringList>
#include <QString>
#include <QCursor>
#include <QPointF>
#include <QRectF>
#include <QList>
#include "CRef.h"
#include "BOILib.h"
#include "UuidType.h"
#include "CRefList.h"
#include "ProfileType.h"
#include "SceneLayerId.h"
#include "ViewTransformerId.h"
#include "CSI.h"


namespace BOI {


class View;
class Scene;
class MenuSet;
class MenuItem;
class TagManager;
class MenuManager;
class Description;
class CRefListPool;
class TransformManager;
class GuiRequestHandler;
class CollectionManager;
class DescriptionManager;


/*
 * Action System Interface: the inteface
 * to the system for actions.
 */

class BOI_LIB_EXPORT ASI
    : public CSI
{
    public:
        ASI(CustomEventsFactory* pCustomEventsFactory,
            DescriptionManager*  pDescriptionManager,
            CollectionManager*   pCollectionManager,
            GuiRequestHandler*   pGuiRequestHandler,
            ComponentManager*    pComponentManager,
            TransformManager*    pTransformManager,
            EventDispatcher*     pEventDispatcher,
            ActionManager*       pActionManager,
            CRefListPool*        pCRefListPool,
            DataManager*         pDataManager,
            MenuManager*         pMenuManager,
            TagManager*          pTagManager,
            MutexBase*           pMutexBase,
            State*               pState,
            Scene*               pScene,
            View*                pView);

        CRef NewComponent(int type, SceneLayerId layer=SceneLayerId_Main);

        const QStringList GetProfile(CRef& cref, ProfileType type=ProfileType_Full);

        CRef ActiveComponent();
        void SetActiveComponent(CRef cref);

        CRef KeyEventHandler();
        void SetKeyEventHandler(CRef cref);

        void CenterComponentOn(CRef& cref, const QPointF& point, bool relativeToScene=false);

        void SetSelectable(CRef& cref, bool selectable);
        void SetPosition(CRef cref, const QPointF& pos);
        void SetRotation(CRef cref, qreal rotation);
        void SetOpacity(CRef cref, qreal opacity);
        void SetVisible(CRef cref, bool visible);
        void SetParent(CRef& cref, CRef& parent);
        void Rotate(CRef& cref, qreal rotation);

        bool IsParent(CRef& parent, CRef& child);
        bool IsParent(Component* pParent, Component* pChild);

        bool IsLayout(CRef& cref);
        void AddToLayout(CRef& cref, CRef& layout);
        void RemoveFromLayout(CRef& cref, CRef& layout);

        int GetCaller(CRef& cref, const QString& uuid);
        int GetFuncSet(CRef& cref, const QString& uuid);
        int GetEmitter(CRef& cref, const QString& uuid);
        int GetReceiver(CRef& cref, const QString& uuid);

        bool ConnectToFuncSet(CRef src, int caller, CRef target, int funcSet);
        bool ConnectToFuncSet(Component* pSrc, int caller, CRef target, int funcSet);

        bool ConnectToReceiver(CRef src, int emitter, CRef target, int receiver);
        bool ConnectToReceiver(Component* pSrc, int emitter, CRef target, int receiver);

        bool DisconnectFromFuncSet(CRef& src, int caller);
        bool DisconnectFromFuncSet(Component* pSrc, int caller);

        void DisconnectFromReceiver(CRef& src, int emitter, CRef& target);
        void DisconnectFromReceiver(Component* pSrc, int emitter, CRef& target);

        DRef CallFunc(CRef& cref, int funcSet, int func, DRef& dref, bool passThru=false);
        DRef CallFunc(Component* pComponent, int funcSet, int func, DRef& dref, bool passThru=false);

        void EmitTo(CRef& cref, int receiver, DRef& data, bool release=false);

        CRefList NewCRefList();

        void ToggleFullScreen();
        void SetFullScreen(bool fullScreen);

        void ToggleHotSpot();

        void SetCursor(const QCursor& cursor);
        void UnsetCursor();

        void TranslateView(qreal x, qreal y);
        void ScaleView(qreal scaleFactor);

        void ResetViewTransform();

        int AppendViewTransformer(ViewTransformerId id,
                                  int version,
                                  qreal* pArgs);

        bool UpdateViewTransformer(int instanceId, qreal* pArgs);

        void StopViewTransformers();
        void StopViewTransformer(int instanceId);

        qreal ViewScaleFactor();

        void CenterViewOn(const QPointF& pos);

        void FitAllInView();

        void AlignViewToScene(const QPoint& viewPoint,
                              const QPointF& scenePoint);

        QPointF MapFromViewToScene(const QPoint& point);
        QPointF MapFromViewToComponent(Component* pComponent, const QPoint& point);
        QPointF MapFromViewToParent(Component* pComponent, const QPoint& point);
        QPointF MapFromViewToParent(CRef& cref, const QPoint& point);

        QPointF MapToParent(CRef& cref, const QPointF& point);

        CRefList ComponentsAtViewPoint(const QPoint& point,
                                       int  sceneLayerIds,
                                       bool visibleOnly=true);

        CRefList ComponentsInLayerRect(const QRectF& rect,
                                       int  sceneLayerIds,
                                       bool visibleOnly=true,
                                       bool intersect=true);

        void StackOnTop(CRef cref);

        const MenuItem* GetMenu(const QString& uuid);

        void GetMenus(CRef& cref, MenuSet& menuSet);

        const Description* GetDescription(const QString& uuidMajor,
                                          const QString& uuidMinor = QString());

        CRefList Selection();
        void SetSelection(CRefList crefList);

        bool IsSelected(CRef& cref);
        bool IsSelected(Component* pComponent);

        QString CreateCollection(CRefList crefs,
                                 const QString& uuid=QString());

        CRefList OpenCollection(const QString& uuid,
                                const QPointF& point);

        bool CollectionExists(const QString& uuid);
        bool DeleteCollection(const QString& uuid);

        bool AddTag(const QString& tag,
                    const QString& uuid,
                    UuidType type);

        QList<TypedUuid> GetTaggedUuids(const QString& tag,
                                        UuidType types=UuidType_All);

        void Shutdown();

    protected:
        View*               m_pView;
        Scene*              m_pScene;
        TagManager*         m_pTagManager;
        MenuManager*        m_pMenuManager;
        CRefListPool*       m_pCRefListPool;
        TransformManager*   m_pTransformManager;
        GuiRequestHandler*  m_pGuiRequestHandler;
        CollectionManager*  m_pCollectionManager;
        DescriptionManager* m_pDescriptionManager;
};


} // namespace BOI


#endif //__BOI_ASI_H

