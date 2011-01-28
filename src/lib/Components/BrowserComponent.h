/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_BROWSERCOMPONENT_H
#define __BOI_BROWSERCOMPONENT_H


#include <QObject>
#include <QWebPage>
#include <QAtomicInt>
#include "Component.h"
#include "StandardEmitters.h"


namespace BOI {


class ActionArgs;
class BrowserComponent;


class BrowserJSBridge
    : public QObject
{
    Q_OBJECT

    public:
        BrowserJSBridge(BrowserComponent* pComponent,
                        QObject* pParent = NULL);

    public slots:
        void EmitString(const QString& string);
        void EmitInt(int value);

        void UpdateAction(const QString& argName,
                          const QString& value);

    private:
        BrowserComponent* m_pComponent;
};


class BrowserObject
    : public QObject
{
    Q_OBJECT

    public:
        BrowserObject(BrowserComponent* pComponent);

        void Draw(QPainter* pPainter, const QRectF& rect);

    public slots:
        void Load(QString url);
        void SetHtml(QString html);
        void EvaluateJavascript(QString script);

        void SetWidth(int width);

        void HandleLoadFinished(bool ok);
        void HandleSizeChanged(const QSize& size);
        void HandleJavascriptWindowObjectCleared();
        void HandleRepaintRequest(const QRect& dirtyRect);

        void HandleMouseMoveEvent(QPoint point);
        void HandleMousePressEvent(QPoint point);
        void HandleMouseReleaseEvent(QPoint point);

        void HandleKeyPressEvent(int key, int modifiers);
        void HandleKeyReleaseEvent(int key, int modifiers);

        void HandleComponentDestroyed();
        void Delete();

    private:
        QWebPage m_webPage;

        BrowserJSBridge m_jsBridge;
        BrowserComponent* m_pComponent;
};


class BrowserObjectController
    : public QObject
{
    Q_OBJECT

    public:
        BrowserObjectController();

        void LoadUrl(const QString& url);
        void SetHtml(const QString& html);
        void EvaluateJavascript(const QString& script);

        void SetPreferredWidth(int width);

        void DeleteBrowserObject();

        void PostMouseMoveEvent(QPoint point);
        void PostMousePressEvent(QPoint point);
        void PostMouseReleaseEvent(QPoint point);

        void PostKeyPressEvent(int key, int modifiers);
        void PostKeyReleaseEvent(int key, int modifiers);

    signals:
        void Load(QString url);
        void Set(QString html);
        void Evaluate(QString script);

        void SetWidth(int width);

        void Delete();

        void MouseMoveEvent(QPoint point);
        void MousePressEvent(QPoint point);
        void MouseReleaseEvent(QPoint point);

        void KeyPressEvent(int key, int modifiers);
        void KeyReleaseEvent(int key, int modifiers);
};


class BrowserComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    BOI_DEFINE_RECEIVER_FUNC(BrowserComponent, LoadUrl);
    BOI_DEFINE_RECEIVER_FUNC(BrowserComponent, SetHtml);
    BOI_DEFINE_RECEIVER_FUNC(BrowserComponent, EvaluateJavascript);

    BOI_DEFINE_RECEIVER_FUNC(BrowserComponent, SetPreferredWidth);

    BOI_DEFINE_FUNCSET_FUNC(BrowserComponent, SetAction);
    BOI_DEFINE_FUNCSET_FUNC(BrowserComponent, SetActionArgs);

    public:
        BrowserComponent();

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual bool HandlesTouchEvents();
        virtual bool HandlesKeyEvents();

        virtual void HandleTouchEvent(TouchEvent* pEvent);
        virtual void HandleKeyEvent(KeyEvent* pEvent);

        virtual int AcceptTransaction(int funcSet);
        virtual void TransactionEnded(int id);

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

    private:
        enum
        {
            Emitter_Default = StandardEmitters::NumEmitters
        };

    private:
        void EmitString(const QString& string);
        void EmitInt(int value);

        void UpdateAction(const QString& argName,
                          const QString& value);

    private:
        friend class BrowserObject;
        friend class BrowserJSBridge;
        BrowserObject* m_pBrowserObject;
        BrowserObjectController m_controller;

        int m_action;
        ActionArgs* m_pActionArgs;

        QAtomicInt m_numTransactions;
};


} // namespace BOI


#endif //__BOI_BROWSERCOMPONENT_H

