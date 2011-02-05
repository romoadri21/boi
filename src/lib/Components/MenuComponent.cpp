/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QStyleOptionGraphicsItem>
#include <QStringList>
#include <QPainter>
#include <QVariant>
#include <QSizeF>
#include <QList>
#include "CSI.h"
#include "DRef.h"
#include "StateId.h"
#include "MenuItem.h"
#include "StandardActions.h"
#include "Events/TouchEvent.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/MenuComponent.h"


#define BOI_MENUCOMPONENT_BACK_TEXT "...Back"


namespace BOI {


BOI_BEGIN_RECEIVERS(MenuComponent)
BOI_END_RECEIVERS(MenuComponent)


BOI_BEGIN_EMITTERS(MenuComponent)
BOI_END_EMITTERS(MenuComponent)


BOI_BEGIN_FUNCSETS(MenuComponent)
    BOI_DECLARE_FUNCSET_6("{1c412c24-6f31-4138-b1f6-b3f4d662fb67}",
                          BOI_FUNCSET_FUNC(MenuComponent, Reset),
                          BOI_FUNCSET_FUNC(MenuComponent, SetRootMenu),
                          BOI_FUNCSET_FUNC(MenuComponent, SetPrimaryContextMenu),
                          BOI_FUNCSET_FUNC(MenuComponent, SetSecondaryContextMenu),
                          BOI_FUNCSET_FUNC(MenuComponent, SetCenterPoint),
                          BOI_FUNCSET_FUNC(MenuComponent, GenerateMenu))
BOI_END_FUNCSETS(MenuComponent)


BOI_BEGIN_CALLERS(MenuComponent)
BOI_END_CALLERS(MenuComponent)


MenuComponent::MenuComponent()
    : Component(BOI_STD_C(Menu)),
      m_windowRect(),
      m_boundingRect(),
      m_centerPoint(),
      m_numTransactions(0),
      m_rootMenu(),
      m_primaryMenu(),
      m_secondaryMenu(),
      m_standardMenu(),
      m_rootMenuSet(),
      m_primaryMenuSet(),
      m_secondaryMenuSet(),
      m_activeMenus(0),
      m_centerGapWidth(23),
      m_centerGapHeight(23),
      m_rootTransform(),
      m_primaryTransform(),
      m_standardTransform(),
      m_secondaryTransform(),
      m_pParentItem(NULL),
      m_pImage(NULL),
      m_borderPen(),
      m_bgFill(),
      m_xRadius(23),
      m_yRadius(23)
{
    m_borderPen.setWidth(10); // TODO: store this in the state, and the radii?
    m_borderPen.setColor(QColor(200, 200, 200));

    m_bgFill.setStyle(Qt::SolidPattern);
    m_bgFill.setColor(QColor(245, 245, 245, 240));
}


bool MenuComponent::Initialize()
{
    QVariant var;
    SI()->RegisterStateListener(StateId_WindowSize, this, var);
    m_windowRect.setSize(var.toSizeF());

    SetFlag(ComponentFlag_HandlesTouchEvents);

    return true;
}


void MenuComponent::Destroy()
{
    SI()->UnregisterStateListener(StateId_WindowSize, this);

    delete m_pImage;
}


void MenuComponent::Draw(QPainter* pPainter,
                         const QStyleOptionGraphicsItem* pOption)
{
    LockDraw();

    if (m_pImage != NULL)
    {
        pPainter->drawImage(pOption->exposedRect,
                            *m_pImage,
                            pOption->exposedRect);
    }

    UnlockDraw();
}


void MenuComponent::HandleItemTouched(const MenuItem* pItem)
{
    if (pItem != NULL)
    {
        if (pItem->Child() != NULL)
        {
            m_pParentItem = pItem;
            GenerateSubMenu();
            Update();
        }
        else
        {
            int action = SI()->ConvertUuid_A(pItem->GetAction());
            if (action != -1)
            {
                SI()->SetActiveAction(action, pItem->GetActionArgs());
            }
            else
            {
                SI()->SetActiveAction(BOI_STD_A(Null));
            }
        }
    }
}


void MenuComponent::HandleTouchEvent(TouchEvent* pTouchEvent)
{
    if (pTouchEvent->type == TouchEvent::Type_Press)
    {
        QPointF point(pTouchEvent->x, pTouchEvent->y);

        if (m_activeMenus & MenuType_Root)
        {
            QPointF mappedPoint = m_rootTransform.inverted().map(point);
            int index = m_rootMenu.IndexAt(mappedPoint);

            if (index != -1)
            {
                const MenuItem* pItem = GetItemAt(&m_rootMenuSet, index);
                HandleItemTouched(pItem);
                return;
            }
        }


        if (m_activeMenus & MenuType_Primary)
        {
            QPointF mappedPoint = m_primaryTransform.inverted().map(point);
            int index = m_primaryMenu.IndexAt(mappedPoint);

            if (index != -1)
            {
                const MenuItem* pItem = GetItemAt(&m_primaryMenuSet, index);
                HandleItemTouched(pItem);
                return;
            }
        }


        if (m_activeMenus & MenuType_Secondary)
        {
            QPointF mappedPoint = m_secondaryTransform.inverted().map(point);
            int index = m_secondaryMenu.IndexAt(mappedPoint);

            if (index != -1)
            {
                const MenuItem* pItem = GetItemAt(&m_secondaryMenuSet, index);
                HandleItemTouched(pItem);
                return;
            }
        }


        if (m_activeMenus & MenuType_Standard)
        {
            QPointF mappedPoint = m_standardTransform.inverted().map(point);
            int index = m_standardMenu.IndexAt(mappedPoint);

            if (index != -1)
            {
                if (m_pParentItem == NULL)
                {
                    const MenuItem* pItem = GetItemAt(&m_primaryMenuSet, index);
                    HandleItemTouched(pItem);
                }
                else
                {
                    /*
                     * In a submenu.
                     */

                    if (index == 0)
                    {
                        /*
                         * The back button was clicked.
                         */

                        m_pParentItem = m_pParentItem->Parent();

                        if (m_pParentItem == NULL) GenerateMenu();
                        else GenerateSubMenu();

                        Update();
                    }
                    else
                    {
                        const MenuItem* pItem = GetItemAt(m_pParentItem->Child(), index-1);
                        HandleItemTouched(pItem);
                    }
                }

                return;
            }
        }

        /*
         * If the menu is clicked in a location which does
         * not correspond to a menu item then set the active
         * action to the NullAction.
         */
        SI()->SetActiveAction(BOI_STD_A(Null));
    }
}


void MenuComponent::HandleStateChanged(StateId stateId, DRef& dref)
{
    if (stateId == StateId_WindowSize)
    {
        // TODO: this can potentially be updated at
        // the same time that UpdatePosition is called
        // from GenerateMenu so set a lock or something.

        m_windowRect.setSize(*dref.GetReadInstance<QSizeF>());
        UpdatePosition();
    }
}


int MenuComponent::AcceptTransaction(int funcSet)
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


void MenuComponent::TransactionEnded(int id)
{
    Q_UNUSED(id);

    m_numTransactions.fetchAndStoreRelaxed(0);
}


DRef MenuComponent::Reset(int id, DRef& dref)
{
    Q_UNUSED(id);
    Q_UNUSED(dref);

    LockDraw();

    m_rootMenuSet.Clear();
    m_primaryMenuSet.Clear();
    m_secondaryMenuSet.Clear();

    m_activeMenus = 0;

    UnlockDraw();

    return DRef();
}


void MenuComponent::SetMenuSet(MenuSet* pMenuSet, DRef& dref)
{
    if (dref.Type() == BOI_STD_D(MenuSet))
    {
        *pMenuSet = *dref.GetReadInstance<MenuSet>();
    }
    else
    {
        pMenuSet->Clear();
    }
}


DRef MenuComponent::SetRootMenu(int id, DRef& dref)
{
    Q_UNUSED(id);

    SetMenuSet(&m_rootMenuSet, dref);
    return DRef();
}


DRef MenuComponent::SetPrimaryContextMenu(int id, DRef& dref)
{
    Q_UNUSED(id);

    SetMenuSet(&m_primaryMenuSet, dref);
    return DRef();
}


DRef MenuComponent::SetSecondaryContextMenu(int id, DRef& dref)
{
    Q_UNUSED(id);

    SetMenuSet(&m_secondaryMenuSet, dref);
    return DRef();
}


DRef MenuComponent::SetCenterPoint(int id, DRef& dref)
{
    Q_UNUSED(id);

    if (dref.Type() == BOI_STD_D(Point))
    {
        m_centerPoint = *dref.GetReadInstance<QPointF>();
    }

    return DRef();
}


DRef MenuComponent::GenerateMenu(int id, DRef& dref)
{
    Q_UNUSED(id);
    Q_UNUSED(dref);

    GenerateMenu();

    return DRef();
}


const MenuItem* MenuComponent::GetItemAt(MenuSet* pMenuSet, int index)
{
    int numMenus = pMenuSet->NumMenus();
    int itemIndex = 0;

    for (int i=0; i < numMenus; i++)
    {
        const MenuItem* pItem = pMenuSet->MenuAt(i);

        while (pItem != NULL)
        {
            if (itemIndex == index)
            {
                return pItem;
            }

            pItem = pItem->Next();
            itemIndex++;
        }
    }

    return NULL;
}


const MenuItem* MenuComponent::GetItemAt(const MenuItem* pItem, int index)
{
    int itemIndex = 0;

    while (pItem != NULL)
    {
        if (itemIndex == index)
        {
            return pItem;
        }

        pItem = pItem->Next();
        itemIndex++;
    }

    return NULL;
}


void MenuComponent::GenerateMenuParams(MenuSet* pMenuSet,
                                       QStringList& items,
                                       QList<int>& sections)
{
    int numMenus = pMenuSet->NumMenus();
    int lastMenuIndex = numMenus - 1;
    int itemIndex = 0;

    for (int i=0; i < numMenus; i++)
    {
        const MenuItem* pItem = pMenuSet->MenuAt(i);

        while (pItem != NULL)
        {
            items.append(pItem->Title());
            pItem = pItem->Next();
            itemIndex++;
        }

        if (i < lastMenuIndex)
        {
            sections.append(itemIndex);
        }
    }

    sections.append(-1);
}


void MenuComponent::GenerateMenuParams(const MenuItem* pItem,
                                       QStringList& items,
                                       QList<int>& sections)
{
    while (pItem != NULL)
    {
        items.append(pItem->Title());
        pItem = pItem->Next();
    }

    sections.append(-1);
}


void MenuComponent::GenerateMenu()
{
    LockDraw();

    m_activeMenus = 0;
    m_pParentItem = NULL;

    if (!m_rootMenuSet.IsEmpty())
    {
        QStringList items;
        QList<int> sections;

        GenerateMenuParams(&m_rootMenuSet, items, sections);

        m_rootMenu.SetItems(items);
        m_rootMenu.SetSections(sections);
        m_rootMenu.Update();

        m_activeMenus |= MenuType_Root;
    }

    if (!m_primaryMenuSet.IsEmpty())
    {
        QStringList items;
        QList<int> sections;

        GenerateMenuParams(&m_primaryMenuSet, items, sections);

        if (m_secondaryMenuSet.IsEmpty())
        {
            m_standardMenu.SetItems(items);
            m_standardMenu.SetSections(sections);
            m_standardMenu.Update();

            m_activeMenus |= MenuType_Standard;
        }
        else
        {
            /*
             * The primary and secondary menus are only
             * used when BOTH their respective menusets
             * contain data. If the primary menuset does
             * not contain data, the secondary menu is
             * not shown.
             */

            m_primaryMenu.SetItems(items);
            m_primaryMenu.SetSections(sections);
            m_primaryMenu.Update();

            items.clear();
            sections.clear();

            GenerateMenuParams(&m_secondaryMenuSet, items, sections);

            m_secondaryMenu.SetItems(items);
            m_secondaryMenu.SetSections(sections);
            m_secondaryMenu.Update();

            m_activeMenus |= (MenuType_Primary | MenuType_Secondary);
        }
    }

    UpdateTransforms();
    CreateImage();
    UpdatePosition();

    SetBoundingRect(m_boundingRect);

    UnlockDraw();
}


void MenuComponent::GenerateSubMenu()
{
    LockDraw();

    if (m_pParentItem != NULL)
    {
        QStringList items;
        QList<int> sections;

        items.append(BOI_MENUCOMPONENT_BACK_TEXT);

        GenerateMenuParams(m_pParentItem->Child(), items, sections);

        m_standardMenu.SetItems(items);
        m_standardMenu.SetSections(sections);
        m_standardMenu.Update();

        m_activeMenus = MenuType_Standard;

        UpdateTransforms();
        CreateImage();
        UpdatePosition();

        SetBoundingRect(m_boundingRect);
    }

    UnlockDraw();
}


void MenuComponent::UpdateTransforms()
{
    qreal width = 0;
    qreal height = 0;

    bool usingContextWidth = false;

    /*
     *  Get the new bounding rect.
     */

    if (m_activeMenus & MenuType_Root)
    {
        width  = m_rootMenu.BoundingRect().width();
        height = m_rootMenu.BoundingRect().height();
    }

    if (m_activeMenus & MenuType_Primary)
    {
        qreal contextWidth  = m_primaryMenu.BoundingRect().width();
        qreal contextHeight = m_primaryMenu.BoundingRect().height();

        contextWidth += m_centerGapWidth;
        contextWidth += m_secondaryMenu.BoundingRect().width();

        if (contextWidth > width)
        {
            width = contextWidth;
            usingContextWidth = true;
        }

        if (m_secondaryMenu.BoundingRect().height() > contextHeight)
        {
            contextHeight = m_secondaryMenu.BoundingRect().height();
        }

        height += contextHeight;
        if (m_activeMenus & MenuType_Root) height += m_centerGapHeight;
    }
    else if (m_activeMenus & MenuType_Standard)
    {
        if (m_standardMenu.BoundingRect().width() > width)
        {
            width = m_standardMenu.BoundingRect().width();
        }

        height += m_standardMenu.BoundingRect().height();
        if (m_activeMenus & MenuType_Root) height += m_centerGapHeight;
    }

    width  += (m_xRadius * 2);
    height += (m_yRadius * 2);

    width  += m_borderPen.width();
    height += m_borderPen.width();

    m_boundingRect.setSize(QSizeF(width, height));
    QPointF center = m_boundingRect.center();

    /*
     * Create the transforms that will correctly
     * position the menus within the new bounding
     * rect.
     */

    qreal halfPenWidth = m_borderPen.width() / 2.0;

    if (m_activeMenus & MenuType_Root)
    {
        QRectF rect = m_rootMenu.BoundingRect();

        qreal centerX = center.x();
        qreal centerY = m_boundingRect.bottom() - m_yRadius;
        centerY -= (rect.height() / 2);
        centerY -= halfPenWidth;

        rect.moveCenter(QPointF(centerX, centerY));

        m_rootTransform.reset();
        m_rootTransform.translate(rect.left(), rect.top());
    }

    if (m_activeMenus & MenuType_Primary)
    {
        QRectF rect = m_primaryMenu.BoundingRect();

        qreal bottom = m_boundingRect.bottom() - m_yRadius;
        bottom -= halfPenWidth;

        if (m_activeMenus & MenuType_Root)
        {
            bottom -= m_centerGapHeight;
            bottom -= m_rootMenu.BoundingRect().height();
        }

        if (usingContextWidth)
        {
            qreal left = m_xRadius + halfPenWidth;
            rect.moveBottomLeft(QPointF(left, bottom));
        }
        else
        {
            qreal right = center.x() - (m_centerGapWidth / 2) + 1;
            rect.moveBottomRight(QPointF(right, bottom));
        }

        m_primaryTransform.reset();
        m_primaryTransform.translate(rect.left(), rect.top());
    }

    if (m_activeMenus & MenuType_Secondary)
    {
        QRectF rect = m_secondaryMenu.BoundingRect();

        qreal bottom = m_boundingRect.bottom() - m_yRadius;
        bottom -= halfPenWidth;

        if (m_activeMenus & MenuType_Root)
        {
            bottom -= m_centerGapHeight;
            bottom -= m_rootMenu.BoundingRect().height();
        }

        if (usingContextWidth)
        {
            qreal right = m_boundingRect.right() - m_xRadius;
            right -= halfPenWidth;
            rect.moveBottomRight(QPointF(right, bottom));
        }
        else
        {
            qreal left = center.x() + (m_centerGapWidth / 2);
            rect.moveBottomLeft(QPointF(left, bottom));
        }

        m_secondaryTransform.reset();
        m_secondaryTransform.translate(rect.left(), rect.top());
    }

    if (m_activeMenus & MenuType_Standard)
    {
        QRectF rect = m_standardMenu.BoundingRect();
        qreal centerX = center.x();
        qreal centerY = m_boundingRect.bottom() - m_yRadius;
        centerY -= (rect.height() / 2);
        centerY -= halfPenWidth;

        if (m_activeMenus & MenuType_Root)
        {
            centerY -= m_centerGapHeight;
            centerY -= m_rootMenu.BoundingRect().height();
        }

        rect.moveCenter(QPointF(centerX, centerY));

        m_standardTransform.reset();
        m_standardTransform.translate(rect.left(), rect.top());
    }
}


void MenuComponent::CreateImage()
{
    if (m_activeMenus)
    {
        QSize newSize = m_boundingRect.size().toSize();

        if (m_pImage != NULL)
        {
            if ((m_pImage->width()  < newSize.width()) ||
                (m_pImage->height() < newSize.height()))
            {
                delete m_pImage;
                m_pImage = new QImage(newSize, QImage::Format_ARGB32);
            }
        }
        else
        {
            m_pImage = new QImage(newSize, QImage::Format_ARGB32);
        }

        m_pImage->fill(0);


        qreal halfPenWidth = m_borderPen.width() / 2;
        QRectF borderRect = m_boundingRect.adjusted(halfPenWidth,
                                                    halfPenWidth,
                                                    -halfPenWidth,
                                                    -halfPenWidth);

        QPainter painter(m_pImage);

        painter.setPen(m_borderPen);
        painter.setBrush(m_bgFill);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawRoundedRect(borderRect, m_xRadius, m_yRadius);
        painter.setRenderHint(QPainter::Antialiasing, false);


        QTransform originalTransform = painter.transform();

        if (m_activeMenus & MenuType_Root)
        {
            QTransform transform = m_rootTransform * originalTransform;
            painter.setTransform(transform);

            m_rootMenu.Draw(&painter, m_rootMenu.BoundingRect());

            painter.setTransform(originalTransform);
        }

        if (m_activeMenus & MenuType_Standard)
        {
            QTransform transform = m_standardTransform * originalTransform;
            painter.setTransform(transform);

            m_standardMenu.Draw(&painter, m_standardMenu.BoundingRect());

            painter.setTransform(originalTransform);
        }

        if (m_activeMenus & MenuType_Primary)
        {
            QTransform transform = m_primaryTransform * originalTransform;
            painter.setTransform(transform);

            m_primaryMenu.Draw(&painter, m_primaryMenu.BoundingRect());

            painter.setTransform(originalTransform);
        }

        if (m_activeMenus & MenuType_Secondary)
        {
            QTransform transform = m_secondaryTransform * originalTransform;
            painter.setTransform(transform);

            m_secondaryMenu.Draw(&painter, m_secondaryMenu.BoundingRect());

            painter.setTransform(originalTransform);
        }
    }
}


void MenuComponent::UpdatePosition()
{
    QRectF rect = m_boundingRect;
    rect.moveCenter(m_centerPoint);

    if (rect.right() > m_windowRect.right())
    {
        rect.moveRight(m_windowRect.right());
    }

    if (rect.bottom() > m_windowRect.bottom())
    {
        rect.moveBottom(m_windowRect.bottom());
    }

    if (rect.left() < 0)
    {
        rect.moveLeft(0);
    }

    if (rect.top() < 0)
    {
        rect.moveTop(0);
    }

    SetPosition(rect.topLeft());
}


} // namespace BOI

