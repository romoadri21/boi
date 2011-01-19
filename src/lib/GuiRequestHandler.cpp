/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QApplication>
#include <QGraphicsScene>
#include <QPointF>
#include <QLineF>
#include <QList>
#include "ISI.h"
#include "View.h"
#include "Component.h"
#include "ComponentData.h"
#include "Utilities/Tuple.h"
#include "StandardEmitters.h"
#include "StandardDataTypes.h"
#include "Events/GuiRequestEvent.h"
#include "Factories/CustomEventsFactory.h"
#include "GuiRequestHandler.h"


namespace BOI {


GuiRequestHandler::GuiRequestHandler()
    : m_guiRequestPool(),
      m_requestQueue(),
      m_pRequestsTail(NULL),
      m_numRequests(0),
      m_mutex(),
      MaxRequestsPerEvent(10),
      m_pCustomEventsFactory(NULL),
      m_pEventReceiver(NULL),
      m_pView(NULL)
{
    m_funcs[GuiRequest::RequestType_MoveToLayer]        = BOI_GUIREQUESTHANDLER_FUNC(MoveToLayer);
    m_funcs[GuiRequest::RequestType_DestroyComponent]   = BOI_GUIREQUESTHANDLER_FUNC(DestroyComponent);
    m_funcs[GuiRequest::RequestType_SetBoundingRect]    = BOI_GUIREQUESTHANDLER_FUNC(SetBoundingRect);
    m_funcs[GuiRequest::RequestType_CenterComponentOn]  = BOI_GUIREQUESTHANDLER_FUNC(CenterComponentOn);
    m_funcs[GuiRequest::RequestType_SetTransformOrigin] = BOI_GUIREQUESTHANDLER_FUNC(SetTransformOrigin);
    m_funcs[GuiRequest::RequestType_SetRotation]        = BOI_GUIREQUESTHANDLER_FUNC(SetRotation);
    m_funcs[GuiRequest::RequestType_SetPosition]        = BOI_GUIREQUESTHANDLER_FUNC(SetPosition);
    m_funcs[GuiRequest::RequestType_StackOnTop]         = BOI_GUIREQUESTHANDLER_FUNC(StackOnTop);
    m_funcs[GuiRequest::RequestType_SetVisible]         = BOI_GUIREQUESTHANDLER_FUNC(SetVisible);
    m_funcs[GuiRequest::RequestType_SetOpacity]         = BOI_GUIREQUESTHANDLER_FUNC(SetOpacity);
    m_funcs[GuiRequest::RequestType_SetParent]          = BOI_GUIREQUESTHANDLER_FUNC(SetParent);
    m_funcs[GuiRequest::RequestType_UpdateRect]         = BOI_GUIREQUESTHANDLER_FUNC(UpdateRect);
    m_funcs[GuiRequest::RequestType_Update]             = BOI_GUIREQUESTHANDLER_FUNC(Update);
    m_funcs[GuiRequest::RequestType_Emit]               = BOI_GUIREQUESTHANDLER_FUNC(Emit);
    m_funcs[GuiRequest::RequestType_Rotate]             = BOI_GUIREQUESTHANDLER_FUNC(Rotate);
}


void GuiRequestHandler::SetCustomEventsFactory(CustomEventsFactory* pFactory)
{
    m_pCustomEventsFactory = pFactory;
}


void GuiRequestHandler::SetEventReceiver(QObject* pReceiver)
{
    m_pEventReceiver = pReceiver;
}


void GuiRequestHandler::SetView(View* pView)
{
    m_pView = pView;
}


bool GuiRequestHandler::PostRequest(GuiRequest* pRequestIn)
{
    bool postEvent = false;

    m_mutex.lock();

    GuiRequest* pRequest = m_guiRequestPool.Get();
    *pRequest = *pRequestIn;

    pRequest->pNext = NULL;

    if ((m_numRequests % MaxRequestsPerEvent) == 0)
    {
        m_pRequestsTail = pRequest;
        m_requestQueue.enqueue(pRequest);
        postEvent = true;
    }
    else
    {
        m_pRequestsTail->pNext = pRequest;
        m_pRequestsTail = pRequest;
    }

    m_numRequests++;

    m_mutex.unlock();

    if (postEvent)
    {
        GuiRequestEvent* pEvent = m_pCustomEventsFactory->NewGuiRequestEvent();
        QApplication::postEvent(m_pEventReceiver, pEvent);
    }

    return true;
}


void GuiRequestHandler::HandleGuiRequestEvent()
{
    ProcessRequests();
}


void GuiRequestHandler::FlushRequests()
{
    while (m_numRequests != 0)
    {
        ProcessRequests();
    }
}


void GuiRequestHandler::ProcessRequests()
{
    GuiRequest* pHeadRequest;
    GuiRequest* pRequest;

    m_mutex.lock();

    Q_ASSERT(!m_requestQueue.isEmpty());
    pHeadRequest = m_requestQueue.dequeue();

    if (m_numRequests > MaxRequestsPerEvent)
    {
        m_numRequests -= MaxRequestsPerEvent;
    }
    else
    {
        m_numRequests = 0;
    }

    m_mutex.unlock();

    pRequest = pHeadRequest;

    while (true)
    {
        GuiRequestFunc func = m_funcs[pRequest->type];
        func(this, pRequest);

        /*
         * Break out of the while loop when pRequest
         * is the last/tail request so that pRequest
         * can be used as a pointer to the tail for
         * GuiRequestPool::Put(...).
         */
        if (pRequest->pNext == NULL) break;

        pRequest = pRequest->pNext;
    }

    m_mutex.lock();
    m_guiRequestPool.Put(pHeadRequest, pRequest);
    m_mutex.unlock();
}


void GuiRequestHandler::MoveToLayer(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        ComponentData* pComponentData = pComponent->m_pData;
        ViewLayerId layer = pRequest->data.viewLayerId;

        GraphicsItem* pItem = &pComponentData->graphicsItem;
 
        m_pView->MoveToLayer(pItem, layer);
        pComponentData->layer = layer;

        UpdateChildLayers(pItem, layer);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::DestroyComponent(GuiRequest* pRequest)
{
    ComponentData* pComponentData = pRequest->data.pComponentData;
    Component* pComponent = pComponentData->pComponent;

    /*
     * Component::Destroy() is called here as opposed to in
     * ComponentManager to avoid issues if the component is drawn
     * before it is destroyed since calling Component::Destroy()
     * may put the Component in a non-drawable state.
     */
    pComponent->Destroy();

    // TODO: is this the best place for this since it can potentially
    // destroy other components and waste valuable time in the GUI thread.
    /* 
     * This should be called after Component::Destroy() so that
     * components can emit from within Component::Destroy().
     */
    pComponentData->connections.Destroy();

    GraphicsItem*   pItem   = &pComponentData->graphicsItem;
    QGraphicsItem*  pParent = pItem->parentItem();
    QGraphicsScene* pScene  = pItem->scene();

    QList<QGraphicsItem*> childItems = pItem->childItems();

    for (int i=0; i < childItems.size(); i++)
    {
        QGraphicsItem* pChild = childItems.at(i);
        pChild->setParentItem(pParent);
    }

    if (pScene != NULL)
    {
        pScene->removeItem(pItem);
    }

    /*
     * Release the internal reference to the Component so that the
     * Component gets deleted when the last reference is released.
     * Note: The cref is first copied locally to avoid an invalid
     * write in CRef::Reset() since CRef::Reset() can delete the
     * Component and thus delete itself (the this pointer).
     */
    CRef cref = pComponentData->cref;
    pComponentData->cref.Reset();
}


void GuiRequestHandler::SetBoundingRect(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        QRectF rect(pRequest->data.rect.x,
                    pRequest->data.rect.y,
                    pRequest->data.rect.width,
                    pRequest->data.rect.height);

        pComponent->m_pData->graphicsItem.SetBoundingRect(rect);

        EmitParentBoundingBox(pComponent, false);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::CenterComponentOn(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        QPointF point(pRequest->data.centerComponentData.x,
                      pRequest->data.centerComponentData.y);

        GraphicsItem* pItem = &pComponent->m_pData->graphicsItem;

        if ((pItem->parentItem() != NULL) &&
            (pRequest->data.centerComponentData.relativeToScene))
        {
            point = pItem->parentItem()->mapFromScene(point);
        }

        point -= pItem->boundingRect().center();
        pItem->setPos(point);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::SetTransformOrigin(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        QPointF pos(pRequest->data.point.x,
                    pRequest->data.point.y);

        pComponent->m_pData->graphicsItem.setTransformOriginPoint(pos);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::SetRotation(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        pComponent->m_pData->graphicsItem.setRotation(pRequest->data.rotation);

        EmitParentBoundingBox(pComponent, false);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::SetPosition(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        QPointF pos(pRequest->data.point.x,
                    pRequest->data.point.y);

        pComponent->m_pData->graphicsItem.setPos(pos);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::StackOnTop(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        GraphicsItem* pItem = &pComponent->m_pData->graphicsItem;
        QGraphicsItem* pParent = pItem->parentItem();

        /*
         * The stacking order is modified based on the following
         * (which is in the Sorting section for the QGraphicsItem
         * documentation): "An item's children are stacked on top
         * of the parent, and sibling items are stacked by insertion
         * order (i.e., in the same order that they were either added
         * to the scene, or added to the same parent).
         */

        pItem->setParentItem(NULL);
        pItem->setParentItem(pParent);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::SetVisible(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        bool curVisible = pComponent->m_pData->visible;
        bool newVisible = pRequest->data.boolean;

        if ((curVisible && !newVisible) ||
            (newVisible && !curVisible))
        {
            pComponent->m_pData->visible = newVisible;
            pComponent->m_pData->graphicsItem.update();
        }

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::SetOpacity(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        /*
         * Note, setOpacity propogates opacity to children
         * unless the children were explicity set to ignore it
         * or the parent is set to not propogate its opacity
         * to children.
         */
        pComponent->m_pData->graphicsItem.setOpacity(pRequest->data.opacity);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::SetParent(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        if (pRequest->cref2.IsValid())
        {
            Component* pParentComponent = pRequest->cref2.GetInstance();
            if (pParentComponent != NULL)
            {
                QGraphicsItem* pChildItem = &pComponent->m_pData->graphicsItem;
                QGraphicsItem* pParentItem = &pParentComponent->m_pData->graphicsItem;

                /*
                 * Don't do parent if pChildItem is an ancestor of
                 * pParentItem (see QGraphicsItem::setParentItem).
                 */
                if (!pChildItem->isAncestorOf(pParentItem) &&
                    (pChildItem != pParentItem))
                {
                    // TODO: handle the case where the parent item
                    // is in a different layer than the child item.

                    SetParent(pChildItem, pParentItem);
                }

                pRequest->cref2.ReleaseInstance();
            }
        }
        else
        {
            QGraphicsItem* pChildItem = &pComponent->m_pData->graphicsItem;
            QGraphicsItem* pTopLevelItem = pChildItem->topLevelItem();

            SetParent(pChildItem, pTopLevelItem);
        }

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
    pRequest->cref2.Reset();
}


void GuiRequestHandler::UpdateRect(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        QRectF rect(pRequest->data.rect.x,
                    pRequest->data.rect.y,
                    pRequest->data.rect.width,
                    pRequest->data.rect.height);

        pComponent->m_pData->graphicsItem.update(rect);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::Update(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        pComponent->m_pData->graphicsItem.update();

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::Emit(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        int  emitter = pRequest->data.emitData.emitter;
        bool newOnly = pRequest->data.emitData.newOnly;

        if (emitter == BOI_STD_E(ParentBoundingBox))
        {
            EmitParentBoundingBox(pComponent, newOnly);
        }

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::EmitParentBoundingBox(Component* pComponent, bool newOnly)
{
    ComponentData* pData = pComponent->m_pData;

    /*
     * Note, the Connections object is used directly instead of the
     * Component object because the Emit methods of the Component
     * object do not support emitting to the standard emitters.
     */
    Connections* pConnections = &pData->connections;

    if (pConnections->EmitterConnected(BOI_STD_E(ParentBoundingBox)))
    {
        typedef Tuple2<QRectF, QPointF> BoundingBox;

        DRef dref = pData->pISI->NewData(BOI_STD_D(BoundingBox));
        BoundingBox* pBoundingBox = dref.GetWriteInstance<BoundingBox>();

        GraphicsItem* pItem = &pData->graphicsItem;
        pBoundingBox->item1 = pItem->mapRectToParent(pItem->boundingRect());
        pBoundingBox->item2 = pItem->mapToParent(QPointF(0, 0));

        pConnections->Emit(BOI_STD_E(ParentBoundingBox), dref, true, newOnly);
    }
}


void GuiRequestHandler::Rotate(GuiRequest* pRequest)
{
    Component* pComponent = pRequest->cref.GetInstance();
    if (pComponent != NULL)
    {
        GraphicsItem* pGraphicsItem = &pComponent->m_pData->graphicsItem;

        qreal rotation = pGraphicsItem->rotation();
        rotation += pRequest->data.rotation;

        pGraphicsItem->setRotation(rotation);

        EmitParentBoundingBox(pComponent, false);

        pRequest->cref.ReleaseInstance();
    }

    pRequest->cref.Reset();
}


void GuiRequestHandler::UpdateChildLayers(GraphicsItem* pParent, ViewLayerId layer)
{
    QList<QGraphicsItem*> pChildren = pParent->childItems();

    for (int i=0; i < pChildren.size(); i++)
    {
        GraphicsItem* pChild = (GraphicsItem*)pChildren.at(i);
        pChild->GetComponent()->m_pData->layer = layer;

        UpdateChildLayers(pChild, layer);
    }
}


void GuiRequestHandler::SetParent(QGraphicsItem* pChild, QGraphicsItem* pParent)
{
    if ((pChild->scene()  != NULL) &&
        (pParent->scene() != NULL))
    {
        /*
         * Maintain the current position of
         * the item relative to the scene.
         */
        QPointF pos = pChild->scenePos();
        pos = pParent->mapFromScene(pos);
        pChild->setPos(pos);

        /*
         * Maintain the current rotation
         * relative to the scene.
         */
        QLineF childLine(pChild->mapToScene(0, 0),
                         pChild->mapToScene(1, 0));
        QLineF parentLine(pParent->mapToScene(0, 0),
                          pParent->mapToScene(1, 0));
        qreal rotation = parentLine.angleTo(childLine);
        pChild->setRotation(rotation * -1);
    }

    pChild->setParentItem(pParent);
}


} // namespace BOI

