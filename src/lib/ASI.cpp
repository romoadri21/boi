/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QList>
#include <QStringList>
#include "State.h"
#include "MenuSet.h"
#include "Component.h"
#include "TagManager.h"
#include "MenuManager.h"
#include "CRefListPool.h"
#include "GraphicsItem.h"
#include "ComponentData.h"
#include "StandardEmitters.h"
#include "TransformManager.h"
#include "ComponentManager.h"
#include "GuiRequestHandler.h"
#include "StandardReceivers.h"
#include "CollectionManager.h"
#include "DescriptionManager.h"
#include "View.h"
#include "ASI.h"


namespace BOI {


ASI::ASI(CustomEventsFactory* pCustomEventsFactory,
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
         View*                pView)
    : CSI(pCustomEventsFactory,
          pComponentManager,
          pEventDispatcher,
          pActionManager,
          pDataManager,
          pMutexBase,
          pState),
      m_pView(pView),
      m_pTagManager(pTagManager),
      m_pMenuManager(pMenuManager),
      m_pCRefListPool(pCRefListPool),
      m_pTransformManager(pTransformManager),
      m_pGuiRequestHandler(pGuiRequestHandler),
      m_pCollectionManager(pCollectionManager),
      m_pDescriptionManager(pDescriptionManager)
{
}


CRef ASI::NewComponent(int type, ViewLayerId layer)
{
    return m_pComponentManager->NewComponent(type, layer);
}


const QStringList ASI::GetProfile(CRef& cref, ProfileType type)
{
    QStringList list;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        list = pComponent->GetProfile(type);
        cref.ReleaseInstance();
    }

    return list;
}


CRef ASI::ActiveComponent()
{
    return m_pState->ActiveComponent();
}


void ASI::SetActiveComponent(CRef cref)
{
    m_pState->SetActiveComponent(cref);
}


CRef ASI::KeyEventHandler()
{
    return m_pState->KeyEventHandler();
}


void ASI::SetKeyEventHandler(CRef cref)
{
    m_pState->SetKeyEventHandler(cref);
}


void ASI::CenterComponentOn(CRef& cref, const QPointF& point, bool relativeToScene)
{
    GuiRequest request;

    request.cref = cref;
    request.data.centerComponentData.x = point.x();
    request.data.centerComponentData.y = point.y();
    request.data.centerComponentData.relativeToScene = relativeToScene;
    request.type = GuiRequest::RequestType_CenterComponentOn;

    m_pGuiRequestHandler->PostRequest(&request);
}


void ASI::SetSelectable(CRef& cref, bool selectable)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        pComponent->m_pData->selectable = selectable;
        cref.ReleaseInstance();
    }
}


void ASI::SetPosition(CRef cref, const QPointF& pos)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        if (!pComponent->m_pData->inLayout)
        {
            GuiRequest request;

            request.cref = cref;
            request.data.point.x = pos.x();
            request.data.point.y = pos.y();
            request.type = GuiRequest::RequestType_SetPosition;

            m_pGuiRequestHandler->PostRequest(&request);
        }

        cref.ReleaseInstance();
    }
}


void ASI::SetRotation(CRef cref, qreal rotation)
{
    GuiRequest request;

    request.cref = cref;
    request.data.rotation = rotation;
    request.type = GuiRequest::RequestType_SetRotation;

    m_pGuiRequestHandler->PostRequest(&request);
}


void ASI::SetOpacity(CRef cref, qreal opacity)
{
    GuiRequest request;

    request.cref = cref;
    request.data.opacity = opacity;
    request.type = GuiRequest::RequestType_SetOpacity;

    m_pGuiRequestHandler->PostRequest(&request);
}


void ASI::SetVisible(CRef cref, bool visible)
{
    GuiRequest request;

    request.cref = cref;
    request.data.boolean = visible;
    request.type = GuiRequest::RequestType_SetVisible;

    m_pGuiRequestHandler->PostRequest(&request);
}


void ASI::SetParent(CRef& cref, CRef& parent)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        if (!pComponent->m_pData->inLayout)
        {
            GuiRequest request;

            request.cref = cref;
            request.cref2 = parent;
            request.type = GuiRequest::RequestType_SetParent;

            m_pGuiRequestHandler->PostRequest(&request);
        }

        cref.ReleaseInstance();
    }
}


void ASI::Rotate(CRef& cref, qreal rotation)
{
    GuiRequest request;

    request.cref = cref;
    request.data.rotation = rotation;
    request.type = GuiRequest::RequestType_Rotate;

    m_pGuiRequestHandler->PostRequest(&request);
}


bool ASI::IsParent(CRef& parent, CRef& child)
{
    bool isParent = false;

    Component* pParent = parent.GetInstance();
    if (pParent != NULL)
    {
        Component* pChild = child.GetInstance();
        if (pChild != NULL)
        {
            if (pChild->m_pData->graphicsItem.parentItem() ==
                &pParent->m_pData->graphicsItem)
            {
                isParent = true;
            }

            child.ReleaseInstance();
        }

        parent.ReleaseInstance();
    }

    return isParent;
}


bool ASI::IsParent(Component* pParent, Component* pChild)
{
    return (pChild->m_pData->graphicsItem.parentItem() ==
            &pParent->m_pData->graphicsItem);
}


bool ASI::IsLayout(CRef& cref)
{
    bool isLayout = false;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        const QStringList profile = pComponent->GetProfile();

        if (profile.contains("{f742f223-bb7b-48f0-92a8-81702e14de16}") &&
            profile.contains("{b11a0db4-cb96-4bf6-9631-fd96f20ea6ab}") &&
            profile.contains("{6a7ab00f-1ab4-4324-9eb4-e614bfca4a16}"))
        {
            isLayout = true;
        }

        cref.ReleaseInstance();
    }

    return isLayout;
}


void ASI::AddToLayout(CRef& cref, CRef& layout)
{
    Component* pLayoutComponent = layout.GetInstance();
    if (pLayoutComponent != NULL)
    {
        int funcSet  = pLayoutComponent->GetFuncSet("{f742f223-bb7b-48f0-92a8-81702e14de16}");
        int emitter  = pLayoutComponent->GetEmitter("{b11a0db4-cb96-4bf6-9631-fd96f20ea6ab}");
        int receiver = pLayoutComponent->GetReceiver("{6a7ab00f-1ab4-4324-9eb4-e614bfca4a16}");

        if ((funcSet  != -1) &&
            (emitter  != -1) &&
            (receiver != -1))
        {
            Component* pComponent = cref.GetInstance();
            if (pComponent != NULL)
            {
                if ((!pComponent->m_pData->inLayout) &&
                    (pComponent != pLayoutComponent))
                {
                    SetParent(cref, layout);

                    /*
                     * Tell the layout component to add
                     * a new item with the given CRef id.
                     */
                    DRef dref = NewData(BOI_STD_D(Int));
                    *dref.GetWriteInstance<int>() = cref.Id();
                    pLayoutComponent->CallFunc(funcSet, 1, dref, true);

                    ConnectToReceiver(pLayoutComponent, emitter, cref, BOI_STD_R(SetPosition));
                    ConnectToReceiver(pComponent, BOI_STD_E(ParentBoundingBox), layout, receiver);

                    pComponent->m_pData->inLayout = true;
                }

                cref.ReleaseInstance();
            }
        }

        layout.ReleaseInstance();
    }
}


void ASI::RemoveFromLayout(CRef& cref, CRef& layout)
{
    Component* pLayoutComponent = layout.GetInstance();
    if (pLayoutComponent != NULL)
    {
        int funcSet  = pLayoutComponent->GetFuncSet("{f742f223-bb7b-48f0-92a8-81702e14de16}");
        int emitter  = pLayoutComponent->GetEmitter("{b11a0db4-cb96-4bf6-9631-fd96f20ea6ab}");
        int receiver = pLayoutComponent->GetReceiver("{6a7ab00f-1ab4-4324-9eb4-e614bfca4a16}");

        if ((funcSet  != -1) &&
            (emitter  != -1) &&
            (receiver != -1))
        {
            Component* pComponent = cref.GetInstance();
            if (pComponent != NULL)
            {
                if (pComponent->m_pData->inLayout &&
                    IsParent(pLayoutComponent, pComponent))
                {
                    /*
                     * Tell the layout component to remove
                     * the item with the given CRef id.
                     */
                    DRef dref = NewData(BOI_STD_D(Int));
                    *dref.GetWriteInstance<int>() = cref.Id();
                    pLayoutComponent->CallFunc(funcSet, 2, dref, true);

                    DisconnectFromReceiver(pLayoutComponent, emitter, cref);
                    DisconnectFromReceiver(pComponent, BOI_STD_E(ParentBoundingBox), layout);

                    pComponent->m_pData->inLayout = false;

                    CRef invalidCRef;
                    SetParent(cref, invalidCRef);
                }

                cref.ReleaseInstance();
            }
        }

        layout.ReleaseInstance();
    }
}


int ASI::GetCaller(CRef& cref, const QString& uuid)
{
    int caller = -1;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        caller = pComponent->GetCaller(uuid);
        cref.ReleaseInstance();
    }

    return caller;
}


int ASI::GetFuncSet(CRef& cref, const QString& uuid)
{
    int funcSet = -1;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        funcSet = pComponent->GetFuncSet(uuid);
        cref.ReleaseInstance();
    }

    return funcSet;
}


int ASI::GetEmitter(CRef& cref, const QString& uuid)
{
    int emitter = -1;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        emitter = pComponent->GetEmitter(uuid);
        cref.ReleaseInstance();
    }

    return emitter;
}


int ASI::GetReceiver(CRef& cref, const QString& uuid)
{
    int receiver = -1;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        receiver = pComponent->GetReceiver(uuid);
        cref.ReleaseInstance();
    }

    return receiver;
}


bool ASI::ConnectToFuncSet(CRef src, int caller, CRef target, int funcSet)
{
    bool connected = false;

    Component* pComponent = src.GetInstance();
    if (pComponent != NULL)
    {
        connected = pComponent->ConnectToFuncSet(caller, target, funcSet);
        src.ReleaseInstance();
    }

    return connected;
}


bool ASI::ConnectToFuncSet(Component* pSrc, int caller, CRef target, int funcSet)
{
    return pSrc->ConnectToFuncSet(caller, target, funcSet);
}


bool ASI::ConnectToReceiver(CRef src, int emitter, CRef target, int receiver)
{
    bool connected = false;

    Component* pComponent = src.GetInstance();
    if (pComponent != NULL)
    {
        connected = pComponent->ConnectToReceiver(emitter, target, receiver);
        src.ReleaseInstance();
    }

    return connected;
}


bool ASI::ConnectToReceiver(Component* pSrc, int emitter, CRef target, int receiver)
{
    return pSrc->ConnectToReceiver(emitter, target, receiver);
}


bool ASI::DisconnectFromFuncSet(CRef& src, int caller)
{
    bool disconnected = true;

    Component* pComponent = src.GetInstance();
    if (pComponent != NULL)
    {
        disconnected = pComponent->DisconnectFromFuncSet(caller);
        src.ReleaseInstance();
    }

    return disconnected;
}


bool ASI::DisconnectFromFuncSet(Component* pSrc, int caller)
{
    return pSrc->DisconnectFromFuncSet(caller);
}


void ASI::DisconnectFromReceiver(CRef& src, int emitter, CRef& target)
{
    Component* pComponent = src.GetInstance();
    if (pComponent != NULL)
    {
        pComponent->DisconnectFromReceiver(emitter, target);
        src.ReleaseInstance();
    }
}


void ASI::DisconnectFromReceiver(Component* pSrc, int emitter, CRef& target)
{
    pSrc->DisconnectFromReceiver(emitter, target);
}


DRef ASI::CallFunc(CRef& cref, int funcSet, int func, DRef& dref, bool passThru)
{
    DRef result;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        result = pComponent->CallFunc(funcSet, func, dref, passThru);
        cref.ReleaseInstance();
    }

    return result;
}


DRef ASI::CallFunc(Component* pComponent, int funcSet, int func, DRef& dref, bool passThru)
{
    return pComponent->CallFunc(funcSet, func, dref, passThru);
}


void ASI::EmitTo(CRef& cref, int receiver, DRef& data, bool release)
{
    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        pComponent->PostToReceiver(receiver, data, release);
        cref.ReleaseInstance();
    }
}


CRefList ASI::NewCRefList()
{
    return m_pCRefListPool->GetList();
}


void ASI::ToggleFullScreen()
{
    m_pView->ToggleFullScreen();
}


void ASI::SetFullScreen(bool fullScreen)
{
    m_pView->SetFullScreen(fullScreen);
}


void ASI::ToggleHotSpot()
{
    m_pView->ToggleHotSpot();
}


void ASI::SetCursor(const QCursor& cursor)
{
    m_pView->SetCursor(cursor);
}


void ASI::UnsetCursor()
{
    m_pView->UnsetCursor();
}


void ASI::TranslateView(qreal x, qreal y)
{
    m_pView->Translate(x, y);
}


void ASI::ScaleView(qreal scaleFactor)
{
    m_pView->Scale(scaleFactor);
}


void ASI::ResetViewTransform()
{
    m_pView->ResetTransform();
}


int ASI::AppendViewTransformer(ViewTransformerId id,
                               int version,
                               qreal* pArgs)
{
    return m_pTransformManager->AppendTransformer(id, version, pArgs);
}


bool ASI::UpdateViewTransformer(int instanceId, qreal* pArgs)
{
    return m_pTransformManager->UpdateTransformer(instanceId, pArgs);
}


void ASI::StopViewTransformers()
{
    m_pTransformManager->StopTransformers();
}


void ASI::StopViewTransformer(int instanceId)
{
    m_pTransformManager->StopTransformer(instanceId);
}


qreal ASI::ViewScaleFactor()
{
    QLineF line = m_pView->Transform().map(QLineF(0,0,1,0));
    return line.length();
}


void ASI::CenterViewOn(const QPointF& pos)
{
    m_pView->CenterOn(pos);
}


void ASI::FitAllInView()
{
    m_pView->FitAllInView();
}


void ASI::AlignLayerToView(const QPointF& layerPoint,
                           const QPointF& viewPoint)
{
    m_pView->AlignLayerToView(layerPoint, viewPoint);
}


QPointF ASI::MapFromViewToLayer(const QPoint& point, ViewLayerId viewLayerId)
{
    return m_pView->MapToLayer(point, viewLayerId);
}


QPointF ASI::MapFromViewToComponent(Component* pComponent, const QPoint& point)
{
    return m_pView->MapToItem(&pComponent->m_pData->graphicsItem, point);
}


QPointF ASI::MapFromViewToParent(Component* pComponent, const QPoint& point)
{
    return m_pView->MapToParent(&pComponent->m_pData->graphicsItem, point);
}


QPointF ASI::MapFromViewToParent(CRef& cref, const QPoint& point)
{
    QPointF mappedPoint;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        mappedPoint = m_pView->MapToParent(&pComponent->m_pData->graphicsItem, point);

        cref.ReleaseInstance();
    }

    return mappedPoint;
}


QPointF ASI::MapToParent(CRef& cref, const QPointF& point)
{
    QPointF mappedPoint;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        GraphicsItem* pItem = &pComponent->m_pData->graphicsItem;
        mappedPoint = pItem->mapToParent(point);

        cref.ReleaseInstance();
    }

    return mappedPoint;
}


CRefList ASI::ComponentsAtViewPoint(const QPoint& point,
                                    int viewLayerIds,
                                    bool visibleOnly)
{
    QList<QGraphicsItem*> items = m_pView->ItemsAt(point, viewLayerIds);
    CRefList list = m_pCRefListPool->GetList();
    GraphicsItem* pGraphicsItem;

    int numItems = items.size();
    for (int i=0; i < numItems; i++)
    {
        pGraphicsItem = (GraphicsItem*)items.at(i);

        /*
         * Note, it is o.k. to temporarily access the component data 
         * directly since the component data is guaranteed to be around
         * at least as long as the GraphicsItem.
         */
        ComponentData* pData = pGraphicsItem->GetComponent()->m_pData;

        if (pData->selectable)
        {
            if (!visibleOnly || pData->visible)
            {
                CRef cref = pData->cref;
                if (!cref.IsDestroyed())
                {
                    list.Append(cref);
                }
            }
        }
    }

    return list;
}


CRefList ASI::ComponentsInLayerRect(const QRectF& rect,
                                    ViewLayerId viewLayerId,
                                    bool visibleOnly,
                                    bool intersect)
{
    QList<QGraphicsItem*> items = m_pView->ItemsIn(rect, viewLayerId, intersect);
    CRefList list = m_pCRefListPool->GetList();
    GraphicsItem* pGraphicsItem;

    int numItems = items.size();
    for (int i=0; i < numItems; i++)
    {
        pGraphicsItem = (GraphicsItem*)items.at(i);

        /*
         * Note, it is o.k. to temporarily access the component data 
         * directly since the component data is guaranteed to be around
         * at least as long as the GraphicsItem.
         */
        ComponentData* pData = pGraphicsItem->GetComponent()->m_pData;

        if (pData->selectable)
        {
            if (!visibleOnly || pData->visible)
            {
                CRef cref = pData->cref;
                if (!cref.IsDestroyed())
                {
                    list.Append(cref);
                }
            }
        }
    }

    return list;
}


void ASI::StackOnTop(CRef cref)
{
    GuiRequest request;

    request.cref = cref;
    request.type = GuiRequest::RequestType_StackOnTop;

    m_pGuiRequestHandler->PostRequest(&request);
}


const MenuItem* ASI::GetMenu(const QString& uuid)
{
    return m_pMenuManager->GetMenu(uuid);
}


void ASI::GetMenus(CRef& cref, MenuSet& menuSet)
{
    int type = cref.Type();

    if (!m_pMenuManager->GetMenus(type, menuSet))
    {
        menuSet.Clear();

        Component* pComponent = cref.GetInstance();
        if (pComponent != NULL)
        {
            const QStringList profile = pComponent->GetProfile();

            m_pMenuManager->GetMenus(profile, menuSet, type);

            cref.ReleaseInstance();
        }
    }
}


const Description* ASI::GetDescription(const QString& uuidMajor,
                                       const QString& uuidMinor)
{
    return m_pDescriptionManager->GetDescription(uuidMajor, uuidMinor);
}


CRefList ASI::Selection()
{
    return m_pState->Selection();
}


void ASI::SetSelection(CRefList crefList)
{
    CRefList prevSelection = m_pState->Selection();

    int numCRefs = prevSelection.Count();
    for (int i=0; i < numCRefs; i++)
    {
        CRef cref = prevSelection.Value(i);

        Component* pComponent = cref.GetInstance();
        if (pComponent != NULL)
        {
            pComponent->m_pData->selected = false;
            pComponent->Update();

            cref.ReleaseInstance();
        }
    }


    numCRefs = crefList.Count();
    for (int i=0; i < numCRefs; i++)
    {
        CRef cref = crefList.Value(i);

        Component* pComponent = cref.GetInstance();
        if (pComponent != NULL)
        {
            pComponent->m_pData->selected = true;
            pComponent->Update();

            cref.ReleaseInstance();
        }
    }

    m_pState->SetSelection(crefList);
}


bool ASI::IsSelected(CRef& cref)
{
    bool selected = false;

    Component* pComponent = cref.GetInstance();
    if (pComponent != NULL)
    {
        selected = pComponent->m_pData->selected;
        cref.ReleaseInstance();
    }

    return selected;
}


bool ASI::IsSelected(Component* pComponent)
{
    return pComponent->m_pData->selected;
}


QString ASI::CreateCollection(CRefList crefs,
                              const QString& uuid)
{
    return m_pCollectionManager->CreateCollection(crefs, uuid);
}


CRefList ASI::OpenCollection(const QString& uuid,
                             const QPointF& point)
{
    return m_pCollectionManager->OpenCollection(uuid, point);
}


bool ASI::CollectionExists(const QString& uuid)
{
    return m_pCollectionManager->CollectionExists(uuid);
}


bool ASI::DeleteCollection(const QString& uuid)
{
    return m_pCollectionManager->DeleteCollection(uuid);
}


bool ASI::AddTag(const QString& tag,
                 const QString& uuid,
                 UuidType type)
{
    return m_pTagManager->AddTag(tag, uuid, type);
}


QList<TypedUuid> ASI::GetTaggedUuids(const QString& tag, UuidType types)
{
    return m_pTagManager->GetUuids(tag, types);
}


void ASI::Shutdown()
{
    m_pView->Close();
}


} // namespace BOI

