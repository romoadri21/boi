/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QTouchEvent>
#include "Events/ResizeEvent.h"
#include "Events/TouchEvent.h"
#include "Events/KeyEvent.h"
#include "Events/EventDispatcher.h"
#include "Events/InputModeChangeEvent.h"
#include "Scene.h"
#include "View.h"


namespace BOI {


View::View(Scene* pScene, QWidget* pParent)
    : QWidget(pParent, 0),
      m_pScene(pScene),
      m_pMainView(NULL),
      m_pSystemView(NULL),
      m_pOverlayView(NULL),
      m_pUnderlayView(NULL),
      m_pEventDispatcher(NULL),
      m_numButtonsPressed(0),
      m_hotSpotTouchId(-1),
      m_hotSpotEnabled(false),
      m_hotSpotBounds()
{
    /*
     * The order in which the views are parented determines
     * their stacking order.
     */
    m_pUnderlayView = new QGraphicsView(this);
    m_pMainView     = new QGraphicsView(this);
    m_pOverlayView  = new QGraphicsView(this);
    m_pSystemView   = new QGraphicsView(this);

    InitializeView(m_pMainView,     m_pScene->Layer(SceneLayerId_Main));
    InitializeView(m_pSystemView,   m_pScene->Layer(SceneLayerId_System));
    InitializeView(m_pOverlayView,  m_pScene->Layer(SceneLayerId_Overlay));
    InitializeView(m_pUnderlayView, m_pScene->Layer(SceneLayerId_Underlay));

    /*
     * It is important that the event filters are installed after
     * everything else is done since the last filter installed is
     * the first one called (see QObject::installEventFilter).
     */
    m_pSystemView->installEventFilter(this);
    m_pSystemView->viewport()->installEventFilter(this);

    /*
     * Combined with eating the 'tab' key events in eventFilter,
     * this will stop the focus from going to the other QGraphicViews
     * and thus prevent removing the keyboard focus from the system
     * view where the eventFilter is installed.
     */
    m_pMainView->setFocusPolicy(Qt::NoFocus);
    m_pOverlayView->setFocusPolicy(Qt::NoFocus);
    m_pUnderlayView->setFocusPolicy(Qt::NoFocus);

    m_pSystemView->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
    m_pSystemView->setFocus();
}


View::~View()
{
    delete m_pMainView;
    delete m_pSystemView;
    delete m_pOverlayView;
    delete m_pUnderlayView;
}


void View::SetEventDispatcher(EventDispatcher* pEventDispatcher)
{
    m_pEventDispatcher = pEventDispatcher;
}


void View::ToggleFullScreen()
{
    if (isFullScreen())
    {
        showNormal();
    }
    else
    {
        showFullScreen();
    }
}


void View::SetFullScreen(bool fullScreen)
{
    if (fullScreen && !isFullScreen())
    {
        showFullScreen();
    }
    else if (!fullScreen && isFullScreen())
    {
        showNormal();
    }
}


void View::EnableHotSpot(bool enable)
{
    m_hotSpotEnabled = enable;
}


void View::InitializeView(QGraphicsView* pView, QGraphicsScene* pScene)
{
    pView->setFrameShape(QFrame::NoFrame);

    /*
     * The underlay view should retain its default background color
     * and have a completely opaque background. This will stop
     * paintEvents from being sent to the main QWidget (aka. this).
     */
    if (pView != m_pUnderlayView)
    {
        QPalette p = pView->viewport()->palette();
        p.setColor(QPalette::Base, Qt::transparent);
        pView->viewport()->setPalette(p);
    }

    pView->setTransformationAnchor(QGraphicsView::NoAnchor);
    pView->setRenderHint(QPainter::Antialiasing);
    pView->setRenderHint(QPainter::SmoothPixmapTransform);
    pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pView->setInteractive(false);
    pView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    pView->setScene(pScene);
    pView->setMouseTracking(false);
}


bool View::eventFilter(QObject *pTarget, QEvent *pEvent)
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

        /*
         * Use the native scan code for the id since
         * the value of key() may not match the key
         * in the corresponding release event. For
         * instance, if key was used as the id:
         *     <shift press>
         *     <colon press>
         *     <shift release>
         *     <semicolon release>,
         * which would set the id in the release as
         * a different value as the id in the press.
         * The id should always match up a key press
         * with its corresponding release.
         */
        keyEvent.id   = ((QKeyEvent*)pEvent)->nativeScanCode();
        keyEvent.key  = ((QKeyEvent*)pEvent)->key();
        keyEvent.type = KeyEvent::Type_Press;

        m_pEventDispatcher->DispatchKeyEvent(&keyEvent);
        return true;
    }
    else if (eventType == QEvent::KeyRelease)
    {
        KeyEvent keyEvent;

        keyEvent.id   = ((QKeyEvent*)pEvent)->nativeScanCode();
        keyEvent.key  = ((QKeyEvent*)pEvent)->key();
        keyEvent.type = KeyEvent::Type_Release;

        m_pEventDispatcher->DispatchKeyEvent(&keyEvent);
        return true;
    }

    return QWidget::eventFilter(pTarget, pEvent);
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


void View::closeEvent(QCloseEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_pEventDispatcher->DispatchCloseEvent();
}


void View::resizeEvent(QResizeEvent* pEvent)
{
    int newWidth = pEvent->size().width();
    int newHeight = pEvent->size().height();

    m_pMainView->setGeometry(0, 0, newWidth, newHeight);
    m_pSystemView->setGeometry(0, 0, newWidth, newHeight);
    m_pOverlayView->setGeometry(0, 0, newWidth, newHeight);
    m_pUnderlayView->setGeometry(0, 0, newWidth, newHeight);

    m_hotSpotBounds = QRectF(0, newHeight-50, 50, 50);

    ResizeEvent resizeEvent;
    resizeEvent.width = newWidth;
    resizeEvent.height = newHeight;

    m_pEventDispatcher->DispatchResizeEvent(&resizeEvent);
}


void View::SetCursor(const QCursor& cursor)
{
    m_pSystemView->setCursor(cursor);
}


void View::UnsetCursor()
{
    m_pSystemView->unsetCursor();
}


void View::Translate(qreal x, qreal y)
{
    m_pMainView->translate(x, y);
}


void View::Scale(qreal scaleFactor)
{
    m_pMainView->scale(scaleFactor, scaleFactor);
}


QRect View::FrameRect()
{
    return m_pSystemView->frameRect();
}


QTransform View::Transform()
{
    return m_pMainView->transform();
}


void View::ResetTransform()
{
    m_pMainView->setTransform(QTransform());
}


void View::CenterOn(const QPointF& pos)
{
    m_pMainView->centerOn(pos);
}


void View::CenterOn(qreal x, qreal y)
{
    m_pMainView->centerOn(QPointF(x, y));
}


void View::FitAllInView()
{
    QGraphicsScene* pScene = m_pScene->Layer(SceneLayerId_Main);
    QRectF bounds = pScene->itemsBoundingRect();

    if (!bounds.isEmpty())
    {
        m_pMainView->fitInView(bounds, Qt::KeepAspectRatio);
    }
}


void View::AlignViewToScene(const QPoint& viewPoint,
                            const QPointF& scenePoint)
{
    QPoint point = m_pMainView->mapFromScene(scenePoint);

    qreal width  = m_pMainView->viewport()->width();
    qreal height = m_pMainView->viewport()->height();

    qreal centerX = width / 2;
    qreal centerY = height / 2;

    qreal xDelta = centerX - viewPoint.x();
    qreal yDelta = centerY - viewPoint.y();

    point.rx() += xDelta;
    point.ry() += yDelta;

    QPointF newSceneCenter = m_pMainView->mapToScene(point);

    m_pMainView->centerOn(newSceneCenter);
}


QPointF View::MapToScene(const QPoint& point)
{
    return m_pMainView->mapToScene(point);
}


QPointF View::MapToItem(QGraphicsItem* pItem, const QPoint& point)
{
    QPointF mappedPoint;
    QGraphicsScene* pScene = pItem->scene();

    if (pScene == m_pScene->Layer(SceneLayerId_Main))
    {
        mappedPoint = m_pMainView->mapToScene(point);
        mappedPoint = pItem->mapFromScene(mappedPoint);
    }
    else
    {
        mappedPoint = pItem->mapFromScene(point);
    }

    return mappedPoint;
}


QPointF View::MapToParent(QGraphicsItem* pItem, const QPoint& point)
{
    QPointF mappedPoint;
    QGraphicsScene* pScene = pItem->scene();

    if (pScene == m_pScene->Layer(SceneLayerId_Main))
    {
        mappedPoint = m_pMainView->mapToScene(point);
        mappedPoint = pItem->mapFromScene(mappedPoint);
    }
    else
    {
        mappedPoint = pItem->mapFromScene(point);
    }

    mappedPoint = pItem->mapToParent(mappedPoint);

    return mappedPoint;
}


QList<QGraphicsItem*> View::ItemsAt(const QPoint& point, int sceneLayerIds)
{
    QGraphicsScene* pScene;
    QList<QGraphicsItem*> list;

    if (sceneLayerIds & SceneLayerId_System)
    {
        pScene = m_pScene->Layer(SceneLayerId_System);
        list.append(pScene->items(point));
    }

    if (sceneLayerIds & SceneLayerId_Overlay)
    {
        pScene = m_pScene->Layer(SceneLayerId_Overlay);
        list.append(pScene->items(point));
    }

    if (sceneLayerIds & SceneLayerId_Main)
    {
        list.append(m_pMainView->items(point));
    }

    if (sceneLayerIds & SceneLayerId_Underlay)
    {
        pScene = m_pScene->Layer(SceneLayerId_Underlay);
        list.append(pScene->items(point));
    }

    return list;
}


} // namespace BOI

