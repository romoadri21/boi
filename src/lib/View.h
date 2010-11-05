/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_VIEW_H
#define __BOI_VIEW_H


#include <QList>
#include <QRect>
#include <QRectF>
#include <QPoint>
#include <QPointF>
#include <QWidget>
#include <QtGlobal>
#include <QTransform>


class QCursor;
class QTouchEvent;
class QGraphicsItem;
class QGraphicsView;
class QGraphicsScene;


namespace BOI {


class Scene;
class TouchEvent;
class EventDispatcher;


class View
    : public QWidget
{
    public:
        View(Scene* pScene, QWidget* pParent=NULL);
        ~View();

        void SetEventDispatcher(EventDispatcher* pEventDispatcher);

        void ToggleFullScreen();
        void SetFullScreen(bool fullScreen);

        void EnableHotSpot(bool enable);

        void SetCursor(const QCursor& cursor);
        void UnsetCursor();

        void Translate(qreal x, qreal y);
        void Scale(qreal scaleFactor);

        QRect FrameRect();
        QTransform Transform();

        void ResetTransform();

        void CenterOn(const QPointF& pos);
        void CenterOn(qreal x, qreal y);

        void FitAllInView();

        void AlignViewToScene(const QPoint& viewPoint,
                              const QPointF& scenePoint);

        QPointF MapToScene(const QPoint& point);
        QPointF MapToItem(QGraphicsItem* pItem, const QPoint& point);
        QPointF MapToParent(QGraphicsItem* pItem, const QPoint& point);

        QList<QGraphicsItem*> ItemsAt(const QPoint& point, int sceneLayerIds);

    protected:
        void InitializeView(QGraphicsView* pView,
                            QGraphicsScene* pScene);

        bool eventFilter(QObject *pTarget, QEvent *pEvent);

        void closeEvent(QCloseEvent* pEvent);
        void resizeEvent(QResizeEvent* pEvent);

        void ProcessQTouchEvent(QTouchEvent* pEvent);
        void HandleTouch(TouchEvent* pEvent);

    private:
        Scene* m_pScene;

        QGraphicsView* m_pMainView;
        QGraphicsView* m_pSystemView;
        QGraphicsView* m_pOverlayView;
        QGraphicsView* m_pUnderlayView;

        EventDispatcher* m_pEventDispatcher;

        int m_numButtonsPressed;

        int m_hotSpotTouchId;
        bool m_hotSpotEnabled;
        QRectF m_hotSpotBounds;
};


} // namespace BOI


#endif //__BOI_VIEW_H

