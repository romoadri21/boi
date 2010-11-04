/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_VERTICALLAYOUTCOMPONENT_H
#define __BOI_VERTICALLAYOUTCOMPONENT_H


#include <QRectF>
#include <QPointF>
#include <QAtomicInt>
#include "DRef.h"
#include "Mutex.h"
#include "StandardEmitters.h"
#include "Component.h"


namespace BOI {


class BasicComponentDrawData;


class VerticalLayoutComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    BOI_DEFINE_RECEIVER_FUNC(VerticalLayoutComponent, ParentBoundingBox);

    BOI_DEFINE_FUNCSET_FUNC(VerticalLayoutComponent, ShowHeader);
    BOI_DEFINE_FUNCSET_FUNC(VerticalLayoutComponent, AddToLayout);
    BOI_DEFINE_FUNCSET_FUNC(VerticalLayoutComponent, RemoveFromLayout);

//    BOI_DEFINE_FUNCSET_FUNC(VerticalLayoutComponent, MoveBefore);
//    BOI_DEFINE_FUNCSET_FUNC(VerticalLayoutComponent, MoveAfter);
//    BOI_DEFINE_FUNCSET_FUNC(VerticalLayoutComponent, SetFirst);
//    BOI_DEFINE_FUNCSET_FUNC(VerticalLayoutComponent, SetLast);

    public:
        VerticalLayoutComponent(const BasicComponentDrawData* pDrawData);

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual int AcceptTransaction(int funcSet);
        virtual void TransactionEnded(int id);

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

    private:
        enum
        {
            Emitter_SetPosition = StandardEmitters::NumEmitters
        };

        typedef struct LayoutItem
        {
            int id;
            QPointF origin;
            QRectF boundingRect;
            LayoutItem* pNext;
            LayoutItem* pPrev;
        } LayoutItem;

    private:
        void UpdatePositions(LayoutItem* pLayoutItem);
        void UpdateBoundingRect();

    private:
        LayoutItem* m_pHead;
        Mutex m_itemsLock;

        bool m_drawHeader;

        QAtomicInt m_numTransactions;

        const BasicComponentDrawData* m_pDrawData;
};


} // namespace BOI


#endif //__BOI_VERTICALLAYOUTCOMPONENT_H

