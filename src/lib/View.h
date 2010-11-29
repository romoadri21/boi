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
#include <QObject>
#include <QtGlobal>
#include <QTransform>
#include "ViewLayerId.h"


class QCursor;
class QWidget;
class QTouchEvent;
class QCloseEvent;
class QResizeEvent;
class QGraphicsItem;
class QGraphicsView;
class QGraphicsScene;


namespace BOI {


class TouchEvent;
class EventDispatcher;


class View
    : public QObject
{
    public:
        View(QWidget* pParent=NULL);
        ~View();

        void SetEventDispatcher(EventDispatcher* pEventDispatcher);

        void SetWindowTitle(const QString& title);
        void Resize(int width, int height);

        void Show();
        void Close();

        void ToggleFullScreen();
        void SetFullScreen(bool fullScreen);

        void ToggleHotSpot();
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

        void AlignLayerToView(const QPointF& layerPoint,
                              const QPointF& viewPoint);

        void MoveToLayer(QGraphicsItem* pItem, ViewLayerId viewLayerId);

        QPointF MapToItem(QGraphicsItem* pItem, const QPoint& point);
        QPointF MapToParent(QGraphicsItem* pItem, const QPoint& point);
        QPointF MapToLayer(const QPoint& point, ViewLayerId viewLayerId);

        QList<QGraphicsItem*> ItemsAt(const QPoint& point, int viewLayerIds);

        QList<QGraphicsItem*> ItemsIn(const QRectF& rect,
                                      ViewLayerId viewLayerId,
                                      bool intersect);

    protected:
        bool eventFilter(QObject* pTarget, QEvent* pEvent);

        void HandleCloseEvent(QCloseEvent* pEvent);
        void HandleResizeEvent(QResizeEvent* pEvent);

        void ProcessQTouchEvent(QTouchEvent* pEvent);
        void HandleTouch(TouchEvent* pEvent);

        ViewLayerId LayerId(QGraphicsItem* pItem);

    private:
        QGraphicsView*  m_pView;
        QGraphicsScene* m_pScene;

        int m_rootItemType;

        QGraphicsItem* m_pMainLayer;
        QGraphicsItem* m_pSystemLayer;
        QGraphicsItem* m_pOverlayLayer;
        QGraphicsItem* m_pUnderlayLayer;

        EventDispatcher* m_pEventDispatcher;

        int m_numButtonsPressed;

        int m_hotSpotTouchId;
        bool m_hotSpotEnabled;
        QRectF m_hotSpotBounds;
};


} // namespace BOI


#endif //__BOI_VIEW_H

