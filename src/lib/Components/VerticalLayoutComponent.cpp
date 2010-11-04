/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "CSI.h"
#include "Utilities/Tuple.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/BasicComponentDrawData.h"
#include "Components/VerticalLayoutComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(VerticalLayoutComponent)
    BOI_DECLARE_RECEIVER("{6a7ab00f-1ab4-4324-9eb4-e614bfca4a16}",
                         BOI_RECEIVER_FUNC(VerticalLayoutComponent, ParentBoundingBox))
BOI_END_RECEIVERS(VerticalLayoutComponent)


BOI_BEGIN_EMITTERS(VerticalLayoutComponent)
    BOI_DECLARE_EMITTER("{b11a0db4-cb96-4bf6-9631-fd96f20ea6ab}")
BOI_END_EMITTERS(VerticalLayoutComponent)


BOI_BEGIN_FUNCSETS(VerticalLayoutComponent)
    BOI_DECLARE_FUNCSET_3("{f742f223-bb7b-48f0-92a8-81702e14de16}",
                          BOI_FUNCSET_FUNC(VerticalLayoutComponent, ShowHeader),
                          BOI_FUNCSET_FUNC(VerticalLayoutComponent, AddToLayout),
                          BOI_FUNCSET_FUNC(VerticalLayoutComponent, RemoveFromLayout))
BOI_END_FUNCSETS(VerticalLayoutComponent)


BOI_BEGIN_CALLERS(VerticalLayoutComponent)
BOI_END_CALLERS(VerticalLayoutComponent)


VerticalLayoutComponent::VerticalLayoutComponent(const BasicComponentDrawData* pDrawData)
    : Component(BOI_STD_C(VerticalLayout)),
      m_pHead(NULL),
      m_itemsLock(),
      m_drawHeader(true),
      m_numTransactions(0),
      m_pDrawData(pDrawData)
{
}


bool VerticalLayoutComponent::Initialize()
{
    SI()->RegisterMutex(&m_itemsLock);

    m_pHead = NULL;

    UpdateBoundingRect();

    return true;
}


void VerticalLayoutComponent::Destroy()
{
    while (m_pHead != NULL)
    {
        LayoutItem* pNext = m_pHead->pNext;
        delete m_pHead;
        m_pHead = pNext;
    }
}


void VerticalLayoutComponent::ParentBoundingBox(BOI::DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(BoundingBox))
    {
        m_itemsLock.Lock();

        LayoutItem* pLayoutItem = m_pHead;

        while (pLayoutItem != NULL)
        {
            if (pLayoutItem->id == source)
            {
                typedef Tuple2<QRectF, QPointF> BoundingBox;
                const BoundingBox* pBoundingBox = dref.GetReadInstance<BoundingBox>();

                pLayoutItem->origin = pBoundingBox->item2;
                pLayoutItem->boundingRect = pBoundingBox->item1;

                UpdatePositions(pLayoutItem);
                UpdateBoundingRect();

                break;
            }

            pLayoutItem = pLayoutItem->pNext;
        }

        m_itemsLock.Unlock();
    }
}


void VerticalLayoutComponent::UpdatePositions(LayoutItem* pLayoutItem)
{
    QPointF point;

    if (pLayoutItem->pPrev != NULL)
    {
        point.ry() = pLayoutItem->pPrev->boundingRect.bottom();
    }
    else if (m_drawHeader)
    {
        point.ry() = m_pDrawData->BoundingRect().bottom();
    }

    DRef dref = SI()->NewData(BOI_STD_D(Point));

    while (pLayoutItem != NULL)
    {
        QPointF delta = point - pLayoutItem->boundingRect.topLeft();
        pLayoutItem->origin += delta;

        pLayoutItem->boundingRect.moveTopLeft(point);

        *dref.GetWriteInstance<QPointF>() = pLayoutItem->origin;
        EmitTo(Emitter_SetPosition, dref, pLayoutItem->id);

        point.ry() += pLayoutItem->boundingRect.height();

        pLayoutItem = pLayoutItem->pNext;
    }
}


void VerticalLayoutComponent::UpdateBoundingRect()
{
    QRectF boundingRect;

    if (m_drawHeader)
    {
        boundingRect = m_pDrawData->BoundingRect();
    }

    LayoutItem* pLayoutItem = m_pHead;

    while (pLayoutItem != NULL)
    {
        boundingRect |= pLayoutItem->boundingRect;
        pLayoutItem = pLayoutItem->pNext;
    }

    SetBoundingRect(boundingRect);
}


int VerticalLayoutComponent::AcceptTransaction(int funcSet)
{
    if (funcSet == 0)
    {
        if (m_numTransactions.testAndSetRelaxed(0, 1))
        {
            return 0;
        }
    }

    return -1;
}


void VerticalLayoutComponent::TransactionEnded(int id)
{
    Q_UNUSED(id);

    m_numTransactions.fetchAndStoreRelaxed(0);
}


DRef VerticalLayoutComponent::ShowHeader(int id, DRef& dref)
{
    Q_UNUSED(id);

    if (dref.Type() == BOI_STD_D(Bool))
    {
        m_itemsLock.Lock();
        LockDraw();

        bool drawHeader = *dref.GetReadInstance<bool>();

        if ((drawHeader && !m_drawHeader) ||
            (!drawHeader && m_drawHeader))
        {
            m_drawHeader = drawHeader;

            if (m_pHead != NULL)
            {
                UpdatePositions(m_pHead);
            }

            UpdateBoundingRect();
            Update(m_pDrawData->BoundingRect());
        }

        UnlockDraw();
        m_itemsLock.Unlock();
    }

    return DRef();
}


DRef VerticalLayoutComponent::AddToLayout(int id, DRef& dref)
{
    Q_UNUSED(id);

    bool alreadyInLayout = false;

    int componentId = *dref.GetReadInstance<int>();

    m_itemsLock.Lock();

    LayoutItem* pLayoutItem = m_pHead;

    while (pLayoutItem != NULL)
    {
        if (pLayoutItem->id == componentId)
        {
            alreadyInLayout = true;
            break;
        }

        if (pLayoutItem->pNext == NULL) break;

        pLayoutItem = pLayoutItem->pNext;
    }

    if (!alreadyInLayout)
    {
        if (pLayoutItem == NULL)
        {
            pLayoutItem = new LayoutItem;
            pLayoutItem->id = componentId;
            pLayoutItem->pNext = NULL;
            pLayoutItem->pPrev = NULL;

            m_pHead = pLayoutItem;

            UpdatePositions(m_pHead);
        }
        else
        {
            LayoutItem* pNewLayoutItem = new LayoutItem;
            pNewLayoutItem->id = componentId;
            pNewLayoutItem->pNext = NULL;
            pNewLayoutItem->pPrev = pLayoutItem;
            pLayoutItem->pNext = pNewLayoutItem;

            UpdatePositions(pNewLayoutItem);
        }

        UpdateBoundingRect();
    }

    m_itemsLock.Unlock();

    return DRef();
}


DRef VerticalLayoutComponent::RemoveFromLayout(int id, DRef& dref)
{
    Q_UNUSED(id);

    int componentId = *dref.GetReadInstance<int>();

    m_itemsLock.Lock();

    LayoutItem* pLayoutItem = m_pHead;

    while (pLayoutItem != NULL)
    {
        if (pLayoutItem->id == componentId)
        {
            if (pLayoutItem->pPrev == NULL)
            {
                m_pHead = pLayoutItem->pNext;
            }
            else
            {
                pLayoutItem->pPrev->pNext = pLayoutItem->pNext;
            }

            if (pLayoutItem->pNext != NULL)
            {
                pLayoutItem->pNext->pPrev = pLayoutItem->pPrev;

                UpdatePositions(pLayoutItem->pNext);
            }

            UpdateBoundingRect();

            delete pLayoutItem;
            break;
        }

        pLayoutItem = pLayoutItem->pNext;
    }

    m_itemsLock.Unlock();

    return DRef();
}


void VerticalLayoutComponent::Draw(QPainter* pPainter,
                                   const QStyleOptionGraphicsItem* pOption)
{
    LockDraw();

    if (m_drawHeader)
    {
        QRectF intersection = m_pDrawData->BoundingRect() & pOption->exposedRect;

        if (!intersection.isEmpty())
        {
            pPainter->drawImage(intersection,
                                m_pDrawData->Image(),
                                intersection);
        }
    }

    UnlockDraw();
}


} // namespace BOI

