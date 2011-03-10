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
#include "Components/HorizontalLayoutComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(HorizontalLayoutComponent)
    BOI_DECLARE_RECEIVER("{6a7ab00f-1ab4-4324-9eb4-e614bfca4a16}",
                         BOI_RECEIVER_FUNC(HorizontalLayoutComponent, ParentBoundingBox))
BOI_END_RECEIVERS(HorizontalLayoutComponent)


BOI_BEGIN_EMITTERS(HorizontalLayoutComponent)
    BOI_DECLARE_EMITTER("{b11a0db4-cb96-4bf6-9631-fd96f20ea6ab}")
BOI_END_EMITTERS(HorizontalLayoutComponent)


BOI_BEGIN_FUNCSETS(HorizontalLayoutComponent)
    BOI_DECLARE_FUNCSET_3("{f742f223-bb7b-48f0-92a8-81702e14de16}",
                          BOI_FUNCSET_FUNC(HorizontalLayoutComponent, ShowHeader),
                          BOI_FUNCSET_FUNC(HorizontalLayoutComponent, AddToLayout),
                          BOI_FUNCSET_FUNC(HorizontalLayoutComponent, RemoveFromLayout))
BOI_END_FUNCSETS(HorizontalLayoutComponent)


BOI_BEGIN_CALLERS(HorizontalLayoutComponent)
BOI_END_CALLERS(HorizontalLayoutComponent)


HorizontalLayoutComponent::HorizontalLayoutComponent(const BasicComponentDrawData* pDrawData)
    : Component(BOI_STD_C(HorizontalLayout)),
      m_pHead(NULL),
      m_itemsLock(),
      m_drawHeader(true),
      m_numTransactions(0),
      m_pDrawData(pDrawData)
{
}


bool HorizontalLayoutComponent::Initialize()
{
    SI()->RegisterMutex(&m_itemsLock);

    m_pHead = NULL;

    UpdateBoundingRect();

    return true;
}


void HorizontalLayoutComponent::Destroy()
{
    while (m_pHead != NULL)
    {
        LayoutItem* pNext = m_pHead->pNext;
        delete m_pHead;
        m_pHead = pNext;
    }
}


void HorizontalLayoutComponent::ParentBoundingBox(BOI::DRef& dref, int source)
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


void HorizontalLayoutComponent::UpdatePositions(LayoutItem* pLayoutItem)
{
    QPointF point;

    if (m_drawHeader)
    {
        point.ry() = m_pDrawData->BoundingRect().bottom();
    }

    if (pLayoutItem->pPrev != NULL)
    {
        point.rx() = pLayoutItem->pPrev->boundingRect.right();
    }

    DRef dref = SI()->NewData(BOI_STD_D(Point));

    while (pLayoutItem != NULL)
    {
        QPointF delta = point - pLayoutItem->boundingRect.topLeft();
        pLayoutItem->origin += delta;

        pLayoutItem->boundingRect.moveTopLeft(point);

        *dref.GetWriteInstance<QPointF>() = pLayoutItem->origin;
        EmitTo(Emitter_SetPosition, dref, pLayoutItem->id);

        point.rx() += pLayoutItem->boundingRect.width();

        pLayoutItem = pLayoutItem->pNext;
    }
}


void HorizontalLayoutComponent::UpdateBoundingRect()
{
    LayoutItem* pLayoutItem = m_pHead;

    QRectF boundingRect;

    if (m_drawHeader)
    {
        boundingRect = m_pDrawData->BoundingRect();
    }

    while (pLayoutItem != NULL)
    {
        boundingRect |= pLayoutItem->boundingRect;
        pLayoutItem = pLayoutItem->pNext;
    }

    SetBoundingRect(boundingRect);
}


int HorizontalLayoutComponent::AcceptTransaction(int funcSet)
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


void HorizontalLayoutComponent::TransactionEnded(int id)
{
    Q_UNUSED(id);

    m_numTransactions.fetchAndStoreRelaxed(0);
}


DRef HorizontalLayoutComponent::ShowHeader(int id, DRef& dref)
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


DRef HorizontalLayoutComponent::AddToLayout(int id, DRef& dref)
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


DRef HorizontalLayoutComponent::RemoveFromLayout(int id, DRef& dref)
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


void HorizontalLayoutComponent::Draw(QPainter* pPainter,
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


void HorizontalLayoutComponent::Import(const QHash<int, DRef>& in)
{
    if (in.contains(1))
    {
        DRef dref = in.value(1);
        const QList<int>* pList = dref.GetReadInstance<QList<int> >();

        int numIds = pList->count();

        LayoutItem* pNewLayoutItem = NULL;
        LayoutItem* pPrevLayoutItem = NULL;

        m_itemsLock.Lock();

        for (int i=0; i < numIds; i++)
        {
            int id = pList->at(i);

            if (id != -1)
            {
                pNewLayoutItem = new LayoutItem;
                pNewLayoutItem->id = id;
                pNewLayoutItem->pNext = NULL;

                if (m_pHead == NULL)
                {
                    m_pHead = pNewLayoutItem;
                    pNewLayoutItem->pPrev = NULL;
                }
                else
                {
                    pNewLayoutItem->pPrev = pPrevLayoutItem;
                    pPrevLayoutItem->pNext = pNewLayoutItem;
                }

                pPrevLayoutItem = pNewLayoutItem;
            }
        }

        m_itemsLock.Unlock();
    }
}


void HorizontalLayoutComponent::Export(QHash<int, DRef>& out)
{
    DRef dref = SI()->NewData(BOI_STD_D(IntList));
    QList<int>* pList = dref.GetWriteInstance<QList<int> >();

    m_itemsLock.Lock();

    LayoutItem* pLayoutItem = m_pHead;

    while (pLayoutItem != NULL)
    {
        pList->append(pLayoutItem->id);
        pLayoutItem = pLayoutItem->pNext;
    }

    m_itemsLock.Unlock();

    out.insert(1, dref);
}


} // namespace BOI

