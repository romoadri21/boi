/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QStyleOptionGraphicsItem>
#include <QApplication>
#include <QWebSettings>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWebFrame>
#include <QString>
#include <QPoint>
#include <QSize>
#include "CSI.h"
#include "ActionArgs.h"
#include "Events/KeyEvent.h"
#include "Events/TouchEvent.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/BrowserComponent.h"


namespace BOI {


BrowserJSBridge::BrowserJSBridge(BrowserComponent* pComponent,
                                 QObject* pParent)
    : QObject(pParent),
      m_pComponent(pComponent)
{
}


void BrowserJSBridge::EmitString(const QString& string)
{
    m_pComponent->EmitString(string);
}


void BrowserJSBridge::EmitInt(int value)
{
    m_pComponent->EmitInt(value);
}


void BrowserJSBridge::UpdateAction(const QString& argName,
                                   const QString& value)
{
    m_pComponent->UpdateAction(argName, value);
}


BrowserObject::BrowserObject(BrowserComponent* pComponent)
    : QObject(),
      m_webPage(this),
      m_jsBridge(pComponent, this),
      m_pComponent(pComponent)
{
    /*
     * Enable plugins for things like Flash content.
     */
    m_webPage.settings()->setAttribute(QWebSettings::PluginsEnabled, true);

    m_webPage.setPreferredContentsSize(QSize(800, 10));

    connect(m_webPage.mainFrame(), SIGNAL(contentsSizeChanged(const QSize&)),
            this, SLOT(HandleSizeChanged(const QSize&)));

    connect(m_webPage.mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(HandleJavascriptWindowObjectCleared()));

    connect(&m_webPage, SIGNAL(repaintRequested(const QRect&)),
            this, SLOT(HandleRepaintRequest(const QRect&)));

    connect(&m_webPage, SIGNAL(loadFinished(bool)),
            this, SLOT(HandleLoadFinished(bool)));
}


void BrowserObject::Load(QString url)
{
    m_webPage.mainFrame()->load(url);
}


void BrowserObject::SetHtml(QString html)
{
    m_webPage.mainFrame()->setHtml(html);
}


void BrowserObject::EvaluateJavascript(QString script)
{
    m_webPage.mainFrame()->evaluateJavaScript(script);
}


void BrowserObject::SetWidth(int width)
{
    m_webPage.setPreferredContentsSize(QSize(width, 10));
}


void BrowserObject::HandleJavascriptWindowObjectCleared()
{
    m_webPage.mainFrame()->addToJavaScriptWindowObject("BOI", &m_jsBridge);
}


void BrowserObject::HandleLoadFinished(bool ok)
{
    Q_UNUSED(ok);

    QSize newSize = m_webPage.mainFrame()->contentsSize();
    m_webPage.setViewportSize(newSize);

    m_pComponent->SetBoundingRect(QRectF(QPointF(0,0), newSize));
    m_pComponent->Update();
}


void BrowserObject::HandleSizeChanged(const QSize& size)
{
    m_webPage.setViewportSize(size);

    m_pComponent->SetBoundingRect(QRectF(QPointF(0,0), size));
    m_pComponent->Update();
}


void BrowserObject::HandleRepaintRequest(const QRect& dirtyRect)
{
    m_pComponent->Update(dirtyRect);
}


void BrowserObject::Draw(QPainter* pPainter, const QRectF& rect)
{
    /*
     * Note, there is no need for a lock around
     * the draw code because the BrowserObject
     * currently lives in the main gui thread.
     */
    m_webPage.mainFrame()->render(pPainter, rect.toRect());
}


void BrowserObject::HandleMouseMoveEvent(QPoint point)
{
    QMouseEvent event(QEvent::MouseMove,
                      point,
                      Qt::NoButton,
                      Qt::NoButton,
                      Qt::NoModifier);

    m_webPage.event(&event);
}


void BrowserObject::HandleMousePressEvent(QPoint point)
{
    QMouseEvent event(QEvent::MouseButtonPress,
                      point,
                      Qt::LeftButton,
                      Qt::LeftButton,
                      Qt::NoModifier);

    m_webPage.event(&event);
}


void BrowserObject::HandleMouseReleaseEvent(QPoint point)
{
    QMouseEvent event(QEvent::MouseButtonRelease,
                      point,
                      Qt::LeftButton,
                      Qt::LeftButton,
                      Qt::NoModifier);

    m_webPage.event(&event);
}


void BrowserObject::HandleKeyPressEvent(int key, int modifiers)
{
    QChar ch(key);

    if (key == Qt::Key_Shift) ch = QChar();

    if ((~modifiers) & KeyEvent::Modifier_Shift)
    {
        ch = ch.toLower();
    }

    QKeyEvent event(QEvent::KeyPress, key, Qt::NoModifier, ch);
    m_webPage.event(&event);
}


void BrowserObject::HandleKeyReleaseEvent(int key, int modifiers)
{
    QChar ch(key);

    if (key == Qt::Key_Shift) ch = QChar();

    if ((~modifiers) & KeyEvent::Modifier_Shift)
    {
        ch = ch.toLower();
    }

    QKeyEvent event(QEvent::KeyRelease, key, Qt::NoModifier, ch);
    m_webPage.event(&event);
}


void BrowserObject::HandleComponentDestroyed()
{
    // TODO: Handle this call and make sure that
    // the component pointer is not dereferenced
    // after the component has been destroyed.
}


void BrowserObject::Delete()
{
    delete this;
}


BrowserObjectController::BrowserObjectController()
    : QObject()
{
}


void BrowserObjectController::LoadUrl(const QString& url)
{
    emit Load(url);
}


void BrowserObjectController::SetHtml(const QString& html)
{
    emit Set(html);
}


void BrowserObjectController::EvaluateJavascript(const QString& script)
{
    emit Evaluate(script);
}


void BrowserObjectController::SetPreferredWidth(int width)
{
    emit SetWidth(width);
}


void BrowserObjectController::DeleteBrowserObject()
{
    emit Delete();
}


void BrowserObjectController::PostMouseMoveEvent(QPoint point)
{
    emit MouseMoveEvent(point);
}


void BrowserObjectController::PostMousePressEvent(QPoint point)
{
    emit MousePressEvent(point);
}


void BrowserObjectController::PostMouseReleaseEvent(QPoint point)
{
    emit MouseReleaseEvent(point);
}


void BrowserObjectController::PostKeyPressEvent(int key, int modifiers)
{
    emit KeyPressEvent(key, modifiers);
}


void BrowserObjectController::PostKeyReleaseEvent(int key, int modifiers)
{
    emit KeyReleaseEvent(key, modifiers);
}


BOI_BEGIN_RECEIVERS(BrowserComponent)
    BOI_DECLARE_RECEIVER("{7473aebc-56e3-4dc4-994e-da426f45866c}",
                         BOI_RECEIVER_FUNC(BrowserComponent, LoadUrl))
    BOI_DECLARE_RECEIVER("{469a8be0-d607-48a0-981f-b41d7a034be5}",
                         BOI_RECEIVER_FUNC(BrowserComponent, SetHtml))
    BOI_DECLARE_RECEIVER("{84235b00-1b96-43bf-907b-240dfc74e6b0}",
                         BOI_RECEIVER_FUNC(BrowserComponent, EvaluateJavascript))
    BOI_DECLARE_RECEIVER("{f0c430fa-ade2-41c8-bba8-f9c6eb03b183}",
                         BOI_RECEIVER_FUNC(BrowserComponent, SetPreferredWidth))
BOI_END_RECEIVERS(BrowserComponent)


BOI_BEGIN_EMITTERS(BrowserComponent)
    BOI_DECLARE_EMITTER("{5f79f90a-0586-45f2-9c9e-53b3143fc475}")
BOI_END_EMITTERS(BrowserComponent)


BOI_BEGIN_FUNCSETS(BrowserComponent)
    BOI_DECLARE_FUNCSET_2("{790e6f3f-4433-4490-a141-a4cb4433b0e7}",
                          BOI_FUNCSET_FUNC(BrowserComponent, SetAction),
                          BOI_FUNCSET_FUNC(BrowserComponent, SetActionArgs))
BOI_END_FUNCSETS(BrowserComponent)


BOI_BEGIN_CALLERS(BrowserComponent)
BOI_END_CALLERS(BrowserComponent)


BrowserComponent::BrowserComponent()
    : Component(BOI_STD_C(Browser)),
      m_controller(),
      m_action(BOI_STD_A(Null)),
      m_pActionArgs(NULL),
      m_numTransactions(0)
{
}


bool BrowserComponent::Initialize()
{
    m_pBrowserObject = new BrowserObject(this);
    m_pBrowserObject->moveToThread(QApplication::instance()->thread());

    QObject::connect(&m_controller,  SIGNAL(Load(QString)),
                     m_pBrowserObject, SLOT(Load(QString)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(Set(QString)),
                     m_pBrowserObject, SLOT(SetHtml(QString)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(Evaluate(QString)),
                     m_pBrowserObject, SLOT(EvaluateJavascript(QString)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(SetWidth(int)),
                     m_pBrowserObject, SLOT(SetWidth(int)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(Delete()),
                     m_pBrowserObject, SLOT(Delete()),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(MouseMoveEvent(QPoint)),
                     m_pBrowserObject, SLOT(HandleMouseMoveEvent(QPoint)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(MousePressEvent(QPoint)),
                     m_pBrowserObject, SLOT(HandleMousePressEvent(QPoint)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(MouseReleaseEvent(QPoint)),
                     m_pBrowserObject, SLOT(HandleMouseReleaseEvent(QPoint)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(KeyPressEvent(int, int)),
                     m_pBrowserObject, SLOT(HandleKeyPressEvent(int, int)),
                     Qt::QueuedConnection);

    QObject::connect(&m_controller,  SIGNAL(KeyReleaseEvent(int, int)),
                     m_pBrowserObject, SLOT(HandleKeyReleaseEvent(int, int)),
                     Qt::QueuedConnection);

    m_controller.LoadUrl("qrc:///BOI/BrowserDefaultPage.html");

    /*
     * Set the bounding rect to the size
     * of the browser default page.
     */
    SetBoundingRect(QRectF(0, 0, 800, 40));

    SetFlag(ComponentFlag_HandlesTouchEvents);
    SetFlag(ComponentFlag_HandlesKeyEvents);

    return true;
}


void BrowserComponent::Destroy()
{
    m_pBrowserObject->HandleComponentDestroyed();

    m_controller.DeleteBrowserObject();
}


void BrowserComponent::HandleTouchEvent(TouchEvent* pTouchEvent)
{
    QPoint point(pTouchEvent->x, pTouchEvent->y);

    if (pTouchEvent->type == TouchEvent::Type_Move)
    {
        m_controller.PostMouseMoveEvent(point);
    }
    else if (pTouchEvent->type == TouchEvent::Type_Press)
    {
        m_controller.PostMousePressEvent(point);
    }
    else if (pTouchEvent->type == TouchEvent::Type_Release)
    {
        m_controller.PostMouseReleaseEvent(point);
    }
}


void BrowserComponent::HandleKeyEvent(KeyEvent* pEvent)
{
    if (pEvent->type == KeyEvent::Type_Press)
    {
        m_controller.PostKeyPressEvent(pEvent->key, pEvent->modifiers);
    }
    else if (pEvent->type == KeyEvent::Type_Release)
    {
        m_controller.PostKeyReleaseEvent(pEvent->key, pEvent->modifiers);
    }
}


int BrowserComponent::AcceptTransaction(int funcSet)
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


void BrowserComponent::TransactionEnded(int id)
{
    Q_UNUSED(id);

    m_numTransactions.fetchAndStoreRelaxed(0);
}


void BrowserComponent::Draw(QPainter* pPainter,
                            const QStyleOptionGraphicsItem* pOption)
{
    /*
     * Note, there is no need for a lock around
     * the draw code because the BrowserObject
     * currently lives in the main gui thread.
     */
    m_pBrowserObject->Draw(pPainter, pOption->exposedRect);
}


void BrowserComponent::LoadUrl(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(String))
    {
        QString url = *dref.GetReadInstance<QString>();
        m_controller.LoadUrl(url);
    }
}


void BrowserComponent::SetHtml(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(String))
    {
        QString html = *dref.GetReadInstance<QString>();
        m_controller.SetHtml(html);
    }
}


void BrowserComponent::EvaluateJavascript(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(String))
    {
        QString script = *dref.GetReadInstance<QString>();
        m_controller.EvaluateJavascript(script);
    }
}


void BrowserComponent::SetPreferredWidth(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Int))
    {
        int width = *dref.GetReadInstance<int>();
        m_controller.SetPreferredWidth(width);
    }
    else if (dref.Type() == BOI_STD_D(Float))
    {
        int width = *dref.GetReadInstance<float>();
        m_controller.SetPreferredWidth(width);
    }
    else if (dref.Type() == BOI_STD_D(Double))
    {
        int width = *dref.GetReadInstance<double>();
        m_controller.SetPreferredWidth(width);
    }
}


void BrowserComponent::EmitString(const QString& string)
{
    if (EmitterConnected(Emitter_Default))
    {
        DRef dref = SI()->NewData(BOI_STD_D(String));
        *dref.GetWriteInstance<QString>() = string;

        Emit(Emitter_Default, dref);
    }
}


void BrowserComponent::EmitInt(int value)
{
    if (EmitterConnected(Emitter_Default))
    {
        DRef dref = SI()->NewData(BOI_STD_D(Int));
        *dref.GetWriteInstance<int>() = value;

        Emit(Emitter_Default, dref);
    }
}


DRef BrowserComponent::SetAction(int id, DRef& dref)
{
    Q_UNUSED(id);

    if (dref.Type() == BOI_STD_D(Int))
    {
        m_action = *dref.GetReadInstance<int>();
    }

    return DRef();
}


DRef BrowserComponent::SetActionArgs(int id, DRef& dref)
{
    Q_UNUSED(id);
    Q_UNUSED(dref);

    // TODO: fill in this method

    return DRef();
}


void BrowserComponent::UpdateAction(const QString& argName,
                                    const QString& value)
{
    ActionArgs* pArgs = (m_pActionArgs != NULL) ?
                        (m_pActionArgs) :
                        (new ActionArgs);
    pArgs->Set(argName, value);

    SI()->UpdateActiveAction(m_action, pArgs);
}


} // namespace BOI

