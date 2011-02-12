/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QFontMetricsF>
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
      m_numNewLines(0),
      m_clearOnNext(false),
      m_windowSize(),
      m_textRect(),
      m_boundingRect(),
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

            if (m_numNewLines > 0)
            {
                m_numNewLines = 0;
                UpdateDimensions();
            }
        }

        int key = pEvent->key;

        if (key == Qt::Key_Backspace)
        {
            if (m_text.endsWith('\n'))
            {
                m_numNewLines--;
                UpdateDimensions();
            }

            m_text.chop(1);
            EmitText();
            Update();
        }
        else if ((key == Qt::Key_Return) ||
                 (key == Qt::Key_Enter))
        {
            if (pEvent->modifiers & KeyEvent::Modifier_Shift)
            {
                m_text.append('\n');
                m_numNewLines++;

                UpdateDimensions();
            }
            else
            {
                ActionArgs* pArgs = (m_pActionArgs != NULL) ?
                                    (m_pActionArgs) :
                                    (new ActionArgs);
                pArgs->Set("Text", m_text);

                SI()->UpdateActiveAction(m_action, pArgs);
            }
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

    QRectF rect = m_textRect;
    rect.adjust(-m_xPadding, -m_yPadding, m_xPadding, m_yPadding);

    pPainter->fillRect(rect, m_textBgFill);

    pPainter->setFont(m_font);
    pPainter->setPen(m_textPen);

    pPainter->drawText(m_textRect, Qt::AlignLeft | Qt::AlignTop, m_text);

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

        int numNewLines = m_text.count('\n');

        if (m_numNewLines != numNewLines)
        {
            m_numNewLines = numNewLines;
            UpdateDimensions();
        }

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
    // TODO: replace the percentages here with values stored in state?

    qreal verticalMargin = m_windowSize.height() * 0.031;
    qreal horizontalMargin = m_windowSize.width() * 0.05;

    m_font.setPointSizeF(m_windowSize.height() * 0.02);

    QFontMetricsF fontMetrics(m_font);
    qreal fontHeight = fontMetrics.height();

    m_textRect.setRect(horizontalMargin,
                       verticalMargin,
                       m_windowSize.width() - (horizontalMargin * 2),
                       fontHeight * (m_numNewLines + 1));

    m_xPadding = fontHeight * 0.75;
    m_yPadding = fontHeight * 0.33;

    m_boundingRect.setRect(0, 0,
                           m_windowSize.width(),
                           m_textRect.height() + (verticalMargin * 2));

    SetPosition(QPointF(0, 0));

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

