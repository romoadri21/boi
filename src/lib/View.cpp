/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QPainter>
#include "Events/ResizeEvent.h"
#include "Events/TouchEvent.h"
#include "Events/KeyEvent.h"
#include "Events/VirtualKeyEvent.h"
#include "Events/EventDispatcher.h"
#include "Events/InputModeChangeEvent.h"
#include "View.h"


namespace BOI {


View::View(QWidget* pParent)
    : m_pView(NULL),
      m_pScene(NULL),
      m_pNullLayer(NULL),
      m_pMainLayer(NULL),
      m_pSystemLayer(NULL),
      m_pOverlayLayer(NULL),
      m_pUnderlayLayer(NULL),
      m_pEventDispatcher(NULL),
      m_numButtonsPressed(0),
      m_hotSpotTouchId(-1),
      m_hotSpotEnabled(false),
      m_hotSpotBounds(),
      m_keyModifiers(0),
      m_shiftPressed(0),
      m_controlPressed(0)
{
    m_pScene = new QGraphicsScene();
    m_pScene->setSceneRect(-30000000, -30000000, 60000000, 60000000);
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    m_pNullLayer = new QGraphicsRectItem(0, 0, 1, 1);

    /*
     * For now, use a QGraphicsRectItem as the parent item
     * for the layers. This can be anything since the item
     * will never be drawn. Note: The order in which the
     * items are inserted into the scene determines their
     * stacking order.
     */
    m_pUnderlayLayer = m_pScene->addRect(QRectF(0,0,1,1));
    m_pMainLayer     = m_pScene->addRect(QRectF(0,0,1,1));
    m_pOverlayLayer  = m_pScene->addRect(QRectF(0,0,1,1));
    m_pSystemLayer   = m_pScene->addRect(QRectF(0,0,1,1));

    m_pMainLayer->setFlag(QGraphicsItem::ItemHasNoContents);
    m_pSystemLayer->setFlag(QGraphicsItem::ItemHasNoContents);
    m_pOverlayLayer->setFlag(QGraphicsItem::ItemHasNoContents);
    m_pUnderlayLayer->setFlag(QGraphicsItem::ItemHasNoContents);

    m_rootItemType = m_pMainLayer->type();

    m_pView = new QGraphicsView(m_pScene, pParent);
    m_pView->setFrameShape(QFrame::NoFrame);
    m_pView->setTransformationAnchor(QGraphicsView::NoAnchor);
    m_pView->setRenderHint(QPainter::Antialiasing);
    m_pView->setRenderHint(QPainter::SmoothPixmapTransform);
    m_pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_pView->setMouseTracking(false);
    m_pView->setInteractive(false);

    /*
     * It is important that the event filters are installed after
     * everything else is done since the last filter installed is
     * the first one called (see QObject::installEventFilter).
     */
    m_pView->installEventFilter(this);
    m_pView->viewport()->installEventFilter(this);

    m_pView->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
    m_pView->setFocus();
}


View::~View()
{
    delete m_pView;
    delete m_pScene;
    delete m_pNullLayer;
}


void View::SetEventDispatcher(EventDispatcher* pEventDispatcher)
{
    m_pEventDispatcher = pEventDispatcher;
}


void View::SetWindowTitle(const QString& title)
{
    m_pView->setWindowTitle(title);
}


void View::Resize(int width, int height)
{
    m_pView->resize(width, height);
}


void View::Show()
{
    m_pView->show();
}


void View::Close()
{
    m_pView->close();
}


void View::ToggleFullScreen()
{
    if (m_pView->isFullScreen())
    {
        m_pView->showNormal();
    }
    else
    {
        m_pView->showFullScreen();
    }
}


void View::SetFullScreen(bool fullScreen)
{
    if (fullScreen && !m_pView->isFullScreen())
    {
        m_pView->showFullScreen();
    }
    else if (!fullScreen && m_pView->isFullScreen())
    {
        m_pView->showNormal();
    }
}


void View::ToggleHotSpot()
{
    m_hotSpotEnabled = !m_hotSpotEnabled;
}


void View::EnableHotSpot(bool enable)
{
    m_hotSpotEnabled = enable;
}


bool View::eventFilter(QObject* pTarget, QEvent* pEvent)
{
    QEvent::Type eventType = pEvent->type();

    if (eventType == QEvent::MouseMove)
    {
        TouchEvent touchEvent;

        touchEvent.x    = ((QMouseEvent*)pEvent)->x();
        touchEvent.y    = ((QMouseEvent*)pEvent)->y();
        touchEvent.id   = 0;
        touchEvent.type = TouchEvent::Type_Move;

        HandleTouch(&touchEvent);
        return true;
    }
    else if (eventType == QEvent::MouseButtonPress)
    {
        if (m_numButtonsPressed == 0)
        {
            if (((QMouseEvent*)pEvent)->button() == Qt::RightButton)
            {
                InputModeChangeEvent event;
                event.mode = InputMode_Gesture;
                m_pEventDispatcher->DispatchInputModeChangeEvent(&event);
            }

            TouchEvent touchEvent;

            touchEvent.x    = ((QMouseEvent*)pEvent)->x();
            touchEvent.y    = ((QMouseEvent*)pEvent)->y();
            touchEvent.id   = 0;
            touchEvent.type = TouchEvent::Type_Press;

            HandleTouch(&touchEvent);
        }

        m_numButtonsPressed++;
        return true;
    }
    else if (eventType == QEvent::MouseButtonRelease)
    {
        if (m_numButtonsPressed > 0)
        {
            /*
             * Only process the mouse release event if the
             * numButtonsPressed is greater than 0 since a
             * double click will send two release events for
             * every one press event which would otherwise
             * make numButtonsPressed negative.
             */

            m_numButtonsPressed--;

            if (m_numButtonsPressed == 0)
            {
                TouchEvent touchEvent;

                touchEvent.x    = ((QMouseEvent*)pEvent)->x();
                touchEvent.y    = ((QMouseEvent*)pEvent)->y();
                touchEvent.id   = 0;
                touchEvent.type = TouchEvent::Type_Release;

                HandleTouch(&touchEvent);
            }
        }

        return true;
    }
    else if ((eventType == QEvent::TouchUpdate) ||
             (eventType == QEvent::TouchBegin) ||
             (eventType == QEvent::TouchEnd))
    {
        ProcessQTouchEvent((QTouchEvent*)pEvent);
        pEvent->setAccepted(true);
        return true;
    }
    else if (eventType == QEvent::KeyPress)
    {
        KeyEvent keyEvent;
        keyEvent.id   = ((QKeyEvent*)pEvent)->key();
        keyEvent.key  = ((QKeyEvent*)pEvent)->key();
        keyEvent.type = KeyEvent::Type_Press;

        ProcessKeyPressEvent(&keyEvent);

        return true;
    }
    else if (eventType == QEvent::KeyRelease)
    {
        KeyEvent keyEvent;
        keyEvent.id   = ((QKeyEvent*)pEvent)->key();
        keyEvent.key  = ((QKeyEvent*)pEvent)->key();
        keyEvent.type = KeyEvent::Type_Release;

        ProcessKeyReleaseEvent(&keyEvent);

        return true;
    }
    else if (eventType == QEvent::Resize)
    {
        HandleResizeEvent((QResizeEvent*)pEvent);
        return false;
    }
    else if (eventType == QEvent::Close)
    {
        HandleCloseEvent((QCloseEvent*)pEvent);
        return true;
    }

    return QObject::eventFilter(pTarget, pEvent);
}


void View::ProcessQTouchEvent(QTouchEvent* pEvent)
{
    const QList<QTouchEvent::TouchPoint>& touchPoints = pEvent->touchPoints();

    for (int i=0; i < touchPoints.size(); i++)
    {
        const QTouchEvent::TouchPoint& touchPoint = touchPoints.at(i);

        if (!touchPoint.isPrimary())
        {
            int state = touchPoint.state();
            QPointF pos = touchPoint.pos();

            TouchEvent touchEvent;
            touchEvent.x  = pos.x();
            touchEvent.y  = pos.y();
            touchEvent.id = touchPoint.id() + 1;

            if (state == Qt::TouchPointMoved)
            {
                touchEvent.type = TouchEvent::Type_Move;
                HandleTouch(&touchEvent);
            }
            else if (state == Qt::TouchPointPressed)
            {
                touchEvent.type = TouchEvent::Type_Press;
                HandleTouch(&touchEvent);
            }
            else if (state == Qt::TouchPointReleased)
            {
                touchEvent.type = TouchEvent::Type_Release;
                HandleTouch(&touchEvent);
            }
        }
    }
}


void View::HandleTouch(TouchEvent* pEvent)
{
    if (m_hotSpotEnabled)
    {
        if (m_hotSpotTouchId == -1)
        {
            if ((pEvent->type == TouchEvent::Type_Press) &&
                (m_hotSpotBounds.contains(pEvent->x, pEvent->y)))
            {
                InputModeChangeEvent event;
                event.mode = InputMode_Gesture;
                m_pEventDispatcher->DispatchInputModeChangeEvent(&event);

                m_hotSpotTouchId = pEvent->id;
            }
            else
            {
                m_pEventDispatcher->DispatchTouchEvent(pEvent);
            }
        }
        else if (pEvent->id != m_hotSpotTouchId)
        {
            /*
             * If the hot spot is touched then only
             * dispatch touch events that are not
             * related to the hot spot.
             */
            m_pEventDispatcher->DispatchTouchEvent(pEvent);
        }
        else if (pEvent->type == TouchEvent::Type_Release)
        {
            /*
             * Note, only hot spot related touch
             * events should get this far. If the
             * event is a release then reset the
             * hot spot touch id.
             */
            m_hotSpotTouchId = -1;
        }
    }
    else
    {
        m_pEventDispatcher->DispatchTouchEvent(pEvent);
    }
}


void View::HandleVirtualKeyEvent(VirtualKeyEvent* pEvent)
{
    int key = (m_keyModifiers & KeyEvent::Modifier_Shift) ?
              (pEvent->shiftKey) :
              (pEvent->defaultKey);

    KeyEvent keyEvent;
    keyEvent.id  = key;
    keyEvent.key = key;

    if (pEvent->type == VirtualKeyEvent::Type_Press)
    {
        keyEvent.type = KeyEvent::Type_Press;
        ProcessKeyPressEvent(&keyEvent);
    }
    else
    {
        keyEvent.type = KeyEvent::Type_Release;
        ProcessKeyReleaseEvent(&keyEvent);
    }
}


void View::ProcessKeyPressEvent(KeyEvent* pEvent)
{
    int key = pEvent->key;

    if (key == Qt::Key_Shift)
    {
        m_keyModifiers |= KeyEvent::Modifier_Shift;
        m_shiftPressed++;
    }
    else if (key == Qt::Key_Control)
    {
        m_keyModifiers |= KeyEvent::Modifier_Control;
        m_controlPressed++;
    }

    pEvent->modifiers = m_keyModifiers;

    m_pEventDispatcher->DispatchKeyEvent(pEvent);

    if ((key != Qt::Key_Shift) &&
        (key != Qt::Key_Control))
    {
        pEvent->type = KeyEvent::Type_Release;
        m_pEventDispatcher->DispatchKeyEvent(pEvent);
    }
}


void View::ProcessKeyReleaseEvent(KeyEvent* pEvent)
{
    int key = pEvent->key;

    if (key == Qt::Key_Shift)
    {
        m_shiftPressed--;

        if (m_shiftPressed == 0)
        {
            m_keyModifiers &= ~(KeyEvent::Modifier_Shift);
        }
    }
    else if (key == Qt::Key_Control)
    {
        m_controlPressed--;

        if (m_controlPressed == 0)
        {
            m_keyModifiers &= ~(KeyEvent::Modifier_Control);
        }
    }

    pEvent->modifiers = m_keyModifiers;

    if ((key == Qt::Key_Shift) ||
        (key == Qt::Key_Control))
    {
        m_pEventDispatcher->DispatchKeyEvent(pEvent);
    }
}


void View::HandleCloseEvent(QCloseEvent* pEvent)
{
    Q_UNUSED(pEvent);
    m_pEventDispatcher->DispatchCloseEvent();
}


void View::HandleResizeEvent(QResizeEvent* pEvent)
{
    int newWidth = pEvent->size().width();
    int newHeight = pEvent->size().height();

    m_hotSpotBounds = QRectF(0, newHeight-50, 50, 50);

    ResizeEvent resizeEvent;
    resizeEvent.width = newWidth;
    resizeEvent.height = newHeight;

    m_pEventDispatcher->DispatchResizeEvent(&resizeEvent);
}


void View::SetCursor(const QCursor& cursor)
{
    m_pView->setCursor(cursor);
}


void View::UnsetCursor()
{
    m_pView->unsetCursor();
}


/*
 * Translates the items in the main
 * layer by (x, y). Note: the input
 * parameters are in the main layers
 * coordinate system.
 */
void View::Translate(qreal x, qreal y)
{
    QPointF pos = m_pMainLayer->pos();

    pos.rx() += (x * m_pMainLayer->scale());
    pos.ry() += (y * m_pMainLayer->scale());

    m_pMainLayer->setPos(pos);
}


void View::Scale(qreal scaleFactor)
{
    qreal scale = m_pMainLayer->scale();
    scale *= scaleFactor;
    m_pMainLayer->setScale(scale);
}


QRect View::FrameRect()
{
    return m_pView->frameRect();
}


QTransform View::Transform()
{
    return m_pMainLayer->sceneTransform();
}


void View::ResetTransform()
{
    m_pMainLayer->setPos(QPointF(0, 0));
    m_pMainLayer->setScale(1.0);
}


void View::CenterOn(const QPointF& pos)
{
    QPointF point = m_pView->frameRect().center();

    AlignLayerToView(pos, point);
}


void View::CenterOn(qreal x, qreal y)
{
    CenterOn(QPointF(x, y));
}


void View::FitAllInView()
{
    QRectF bounds = m_pMainLayer->childrenBoundingRect();

    FitRectInView(bounds);
}


void View::FitRectInView(const QRectF& rect)
{
    if (!rect.isEmpty())
    {
        QRectF viewBounds = m_pView->frameRect();

        qreal xScale = viewBounds.width() / rect.width();
        qreal yScale = viewBounds.height() / rect.height();
        qreal scale = (xScale < yScale) ? xScale : yScale;

        m_pMainLayer->setScale(scale);

        AlignLayerToView(rect.topLeft(), QPoint(0, 0));
    }
}


void View::AlignLayerToView(const QPointF& layerPoint,
                            const QPointF& viewPoint)
{
    QPointF point = viewPoint - (layerPoint * m_pMainLayer->scale());

    m_pMainLayer->setPos(point);
}


void View::MoveToLayer(QGraphicsItem* pItem, ViewLayerId viewLayerId)
{
    QGraphicsItem* newParent = m_pNullLayer;

    if (viewLayerId == ViewLayerId_Null) newParent = m_pNullLayer;
    else if (viewLayerId == ViewLayerId_Main) newParent = m_pMainLayer;
    else if (viewLayerId == ViewLayerId_System) newParent = m_pSystemLayer;
    else if (viewLayerId == ViewLayerId_Overlay) newParent = m_pOverlayLayer;
    else if (viewLayerId == ViewLayerId_Underlay) newParent = m_pUnderlayLayer;

    if (pItem->scene() != NULL)
    {
        /*
         * Maintain the current position of
         * the item relative to the scene.
         */
        QPointF pos = pItem->scenePos();
        pos = newParent->mapFromScene(pos);
        pItem->setPos(pos);
    }

    pItem->setParentItem(newParent);
}


QRectF View::MapToLayer(const QRectF& rect, ViewLayerId viewLayerId)
{
    if (viewLayerId == ViewLayerId_Main)
    {
        return m_pMainLayer->mapRectFromScene(rect);
    }
    else
    {
        return rect;
    }
}


QPointF View::MapToLayer(const QPoint& point, ViewLayerId viewLayerId)
{
    if (viewLayerId == ViewLayerId_Main)
    {
        return m_pMainLayer->mapFromScene(point);
    }
    else
    {
        return point;
    }
}


QRectF View::MapFromLayer(const QRectF& rect, ViewLayerId viewLayerId)
{
    if (viewLayerId == ViewLayerId_Main)
    {
        return m_pMainLayer->mapRectToScene(rect);
    }
    else
    {
        return rect;
    }
}


QPointF View::MapToItem(QGraphicsItem* pItem, const QPoint& point)
{
    return pItem->mapFromScene(point);
}


QPointF View::MapToParent(QGraphicsItem* pItem, const QPoint& point)
{
    /*
     * Note, it is guaranteed that there will
     * be a parent item since all components
     * are children of a root layer item.
     */
    return pItem->parentItem()->mapFromScene(point);
}


ViewLayerId View::LayerId(QGraphicsItem* pItem)
{
    QGraphicsItem* pTopLevelItem = pItem->topLevelItem();

    if (pTopLevelItem == m_pMainLayer) return ViewLayerId_Main;
    else if (pTopLevelItem == m_pSystemLayer) return ViewLayerId_System;
    else if (pTopLevelItem == m_pOverlayLayer) return ViewLayerId_Overlay;
    else if (pTopLevelItem == m_pUnderlayLayer) return ViewLayerId_Underlay;

    return ViewLayerId_Null;
}


QList<QGraphicsItem*> View::ItemsAt(const QPoint& point, int viewLayerIds)
{
    QList<QGraphicsItem*> items = m_pScene->items(point);
    QList<QGraphicsItem*> filteredItems;

    for (int i=0; i < items.size(); i++)
    {
        QGraphicsItem* pItem = items.at(i);

        if (pItem->type() != m_rootItemType)
        {
            if (LayerId(pItem) & viewLayerIds)
            {
                filteredItems.append(pItem);
            }
        }
    }

    return filteredItems;
}


QList<QGraphicsItem*> View::ItemsIn(const QRectF& rect,
                                    ViewLayerId viewLayerId,
                                    bool intersect)
{
    Qt::ItemSelectionMode mode = (intersect) ?
                                 (Qt::IntersectsItemShape) :
                                 (Qt::ContainsItemShape);

    QList<QGraphicsItem*> items;
    QList<QGraphicsItem*> filteredItems;

    if (viewLayerId == ViewLayerId_Main)
    {
        QRectF mappedRect = m_pMainLayer->mapRectToScene(rect);
        items = m_pScene->items(mappedRect, mode);
    }
    else
    {
        items = m_pScene->items(rect, mode);
    }


    for (int i=0; i < items.size(); i++)
    {
        QGraphicsItem* pItem = items.at(i);

        if (pItem->type() != m_rootItemType)
        {
            if (LayerId(pItem) == viewLayerId)
            {
                filteredItems.append(pItem);
            }
        }
    }

    return filteredItems;
}


QImage View::Capture(const QRectF& rect)
{
    m_pSystemLayer->setVisible(false);
    m_pOverlayLayer->setVisible(false);
    m_pUnderlayLayer->setVisible(false);

    QTransform transform = m_pMainLayer->transform();
    m_pMainLayer->resetTransform();

    QRectF sourceRect = m_pMainLayer->mapRectToScene(rect);
    QRectF targetRect(0, 0, rect.width(), rect.height());

    QImage image(targetRect.width(),
                 targetRect.height(),
                 QImage::Format_ARGB32);
    image.fill(0);

    QPainter painter(&image);

    m_pScene->render(&painter, targetRect, sourceRect);

    m_pMainLayer->setTransform(transform);

    m_pUnderlayLayer->setVisible(true);
    m_pOverlayLayer->setVisible(true);
    m_pSystemLayer->setVisible(true);

    return image;
}


} // namespace BOI

