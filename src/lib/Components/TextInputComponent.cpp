/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPainter>
#include <QVariant>
#include "CSI.h"
#include "ActionArgs.h"
#include "Events/KeyEvent.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/TextInputComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(TextInputComponent)
    BOI_DECLARE_RECEIVER("{7f2249e4-6c3c-40a5-9cff-59501f06ee37}",
                         BOI_RECEIVER_FUNC(TextInputComponent, SetText))
    BOI_DECLARE_RECEIVER("{f58a0c2e-63c7-41e4-8895-6c7fa44ae32f}",
                         BOI_RECEIVER_FUNC(TextInputComponent, ClearOnNextPress))
BOI_END_RECEIVERS(TextInputComponent)


BOI_BEGIN_EMITTERS(TextInputComponent)
    BOI_DECLARE_EMITTER("{3518d488-5339-49a4-b927-bb69d7d9cf18}")
BOI_END_EMITTERS(TextInputComponent)


BOI_BEGIN_FUNCSETS(TextInputComponent)
    BOI_DECLARE_FUNCSET_2("{790e6f3f-4433-4490-a141-a4cb4433b0e7}",
                          BOI_FUNCSET_FUNC(TextInputComponent, SetAction),
                          BOI_FUNCSET_FUNC(TextInputComponent, SetActionArgs))
BOI_END_FUNCSETS(TextInputComponent)


BOI_BEGIN_CALLERS(TextInputComponent)
BOI_END_CALLERS(TextInputComponent)


TextInputComponent::TextInputComponent()
    : Component(BOI_STD_C(TextInput)),
      m_text(),
      m_clearOnNext(false),
      m_windowSize(),
      m_boundingRect(),
      m_innerBoundingRect(),
      m_textPen(),
      m_bgFill(),
      m_textBgFill(),
      m_font(),
      m_action(BOI_STD_A(Null)),
      m_pActionArgs(NULL),
      m_numTransactions(0)
{
}


bool TextInputComponent::Initialize()
{
    QVariant value;
    SI()->RegisterStateListener(StateId_WindowSize, this, value);
    m_windowSize = value.toSizeF();

    // TODO: get these values from State.
    m_textPen = QPen(QColor(50, 50, 50));
    m_bgFill = QBrush(QColor(200, 200, 200));
    m_textBgFill = QBrush(QColor(245, 245, 245));

    UpdateDimensions();

    SetFlag(ComponentFlag_HandlesKeyEvents);

    return true;
}


void TextInputComponent::Destroy()
{
    SI()->UnregisterStateListener(StateId_WindowSize, this);
}


void TextInputComponent::HandleStateChanged(StateId stateId, DRef& dref)
{
    if (stateId == StateId_WindowSize)
    {
        const QSizeF* pSize = dref.GetReadInstance<QSizeF>();

        LockDraw();

        m_windowSize = *pSize;
        UpdateDimensions();

        UnlockDraw();
    }
}


void TextInputComponent::HandleKeyEvent(KeyEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == KeyEvent::Type_Press)
    {
        LockDraw();

        if (m_clearOnNext)
        {
            m_text.clear();
            m_clearOnNext = false;
        }

        int key = pEvent->key;

        if (key == Qt::Key_Backspace)
        {
            m_text.chop(1);
            EmitText();
            Update();
        }
        else if ((key == Qt::Key_Return) ||
                 (key == Qt::Key_Enter))
        {
            ActionArgs* pArgs = (m_pActionArgs != NULL) ?
                                (m_pActionArgs) :
                                (new ActionArgs);
            pArgs->Set("Text", m_text);

            SI()->UpdateActiveAction(m_action, pArgs);
        }
        else if ((key != Qt::Key_Shift) &&
                 (key != Qt::Key_Control))
        {
            QChar ch(key);

            if (ch.isPrint())
            {
                if (pEvent->modifiers & KeyEvent::Modifier_Shift)
                {
                    m_text.append(ch);
                }
                else
                {
                    m_text.append(ch.toLower());
                }

                EmitText();
                Update();
            }
        }

        UnlockDraw();
    }
}


int TextInputComponent::AcceptTransaction(int funcSet)
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


void TextInputComponent::TransactionEnded(int id)
{
    Q_UNUSED(id);

    m_numTransactions.fetchAndStoreRelaxed(0);
}


void TextInputComponent::Draw(QPainter* pPainter,
                              const QStyleOptionGraphicsItem* pOption)
{
    Q_UNUSED(pOption);

    LockDraw();

    pPainter->fillRect(m_boundingRect, m_bgFill);
    pPainter->fillRect(m_innerBoundingRect, m_textBgFill);

    pPainter->setFont(m_font);
    pPainter->setPen(m_textPen);

    QRectF textRect = m_innerBoundingRect;
    textRect.setX(m_innerBoundingRect.x() + (m_innerBoundingRect.height() / 2));
    pPainter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);

    UnlockDraw();
}


void TextInputComponent::EmitText()
{
    if (EmitterConnected(Emitter_Text))
    {
        DRef dref = SI()->NewData(BOI_STD_D(String));
        *dref.GetWriteInstance<QString>() = m_text;

        Emit(Emitter_Text, dref, true);
    }
}


void TextInputComponent::SetText(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(String))
    {
        QString text = *dref.GetReadInstance<QString>();

        LockDraw();

        m_text = text;

        UnlockDraw();

        Update();
    }
}


void TextInputComponent::ClearOnNextPress(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(Bool))
    {
        m_clearOnNext = *dref.GetReadInstance<bool>();
    }
}


void TextInputComponent::UpdateDimensions()
{
    // TODO: replace the percentages here with values stored in state.

    qreal width = m_windowSize.width() * 0.85;
    qreal height = m_windowSize.height() * 0.05;
    qreal leftMargin = m_windowSize.width() * ((1.0 - 0.85) / 2.0);

    m_boundingRect.setRect(0, 0, m_windowSize.width(), height*2);
    m_innerBoundingRect.setRect(leftMargin, (height / 2), width, height);

    SetPosition(QPointF(0, 0));

    m_font.setPixelSize(height * 0.5);

    SetBoundingRect(m_boundingRect);
    Update();
}


DRef TextInputComponent::SetAction(int id, DRef& dref)
{
    Q_UNUSED(id);

    if (dref.Type() == BOI_STD_D(Int))
    {
        m_action = *dref.GetReadInstance<int>();
    }

    return DRef();
}


DRef TextInputComponent::SetActionArgs(int id, DRef& dref)
{
    Q_UNUSED(id);
    Q_UNUSED(dref);

    // TODO: fill in this method

    return DRef();
}


} // namespace BOI

