/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MENUCOMPONENT_H
#define __BOI_MENUCOMPONENT_H


#include <QPen>
#include <QRectF>
#include <QBrush>
#include <QPointF>
#include <QTransform>
#include <QAtomicInt>
#include "MenuSet.h"
#include "Components/RootMenu.h"
#include "Components/StandardMenu.h"
#include "Component.h"


class QImage;


namespace BOI {


class MenuItem;


class MenuComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    BOI_DEFINE_FUNCSET_FUNC(MenuComponent, Reset);
    BOI_DEFINE_FUNCSET_FUNC(MenuComponent, SetRootMenu);
    BOI_DEFINE_FUNCSET_FUNC(MenuComponent, SetPrimaryContextMenu);
    BOI_DEFINE_FUNCSET_FUNC(MenuComponent, SetSecondaryContextMenu);
    BOI_DEFINE_FUNCSET_FUNC(MenuComponent, SetCenterPoint);
    BOI_DEFINE_FUNCSET_FUNC(MenuComponent, GenerateMenu);

    public:
        MenuComponent();

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

        virtual bool HandlesTouchEvents();
        virtual void HandleTouchEvent(TouchEvent* pEvent);

        virtual void HandleStateChanged(StateId stateId, DRef& dref);

        virtual int AcceptTransaction(int funcSet);
        virtual void TransactionEnded(int id);

    private:
        enum MenuType
        {
            MenuType_Root = 0x1,
            MenuType_Primary = 0x2,
            MenuType_Standard = 0x4,
            MenuType_Secondary = 0x8
        };

    private:
        void SetMenuSet(MenuSet* pMenuSet, DRef& dref);

        void GenerateMenuParams(MenuSet* pMenuSet,
                                QStringList& items,
                                QList<int>& sections);

        void GenerateMenuParams(const MenuItem* pItem,
                                QStringList& items,
                                QList<int>& sections);

        void GenerateMenu();
        void GenerateSubMenu();

        void UpdateTransforms();
        void UpdatePosition();

        void CreateImage();

        const MenuItem* GetItemAt(MenuSet* pMenuSet, int index);
        const MenuItem* GetItemAt(const MenuItem* pItem, int index);

        void HandleItemTouched(const MenuItem* pItem);

    private:
        QRectF m_windowRect;
        QRectF m_boundingRect;

        QPointF m_centerPoint;

        QAtomicInt m_numTransactions;

        RootMenu     m_rootMenu;
        StandardMenu m_primaryMenu;
        StandardMenu m_secondaryMenu;
        StandardMenu m_standardMenu;

        MenuSet m_rootMenuSet;
        MenuSet m_primaryMenuSet;
        MenuSet m_secondaryMenuSet;

        int m_activeMenus;

        qreal m_centerGapWidth;
        qreal m_centerGapHeight;

        QTransform m_rootTransform;
        QTransform m_primaryTransform;
        QTransform m_standardTransform;
        QTransform m_secondaryTransform;

        const MenuItem* m_pParentItem;

        QImage* m_pImage;

        QPen m_borderPen;
        QBrush m_bgFill;

        qreal m_xRadius;
        qreal m_yRadius;
};


} // namespace BOI


#endif //__BOI_MENUCOMPONENT_H

