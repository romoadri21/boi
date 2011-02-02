/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include "CSI.h"
#include "Events/KeyEvent.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/TextComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(TextComponent)
    BOI_DECLARE_RECEIVER("{7f2249e4-6c3c-40a5-9cff-59501f06ee37}",
                         BOI_RECEIVER_FUNC(TextComponent, SetText))
    BOI_DECLARE_RECEIVER("{b01ea5e1-a4b9-4114-873f-376ab652a554}",
                         BOI_RECEIVER_FUNC(TextComponent, AppendText))
    BOI_DECLARE_RECEIVER("{fb120d04-b81a-41cb-bb48-ca0295b4498f}",
                         BOI_RECEIVER_FUNC(TextComponent, InsertText))
    BOI_DECLARE_RECEIVER("{b7b98851-6c34-4fa7-beff-2b98aea6041f}",
                         BOI_RECEIVER_FUNC(TextComponent, SetFontSize))
BOI_END_RECEIVERS(TextComponent)


BOI_BEGIN_EMITTERS(TextComponent)
    BOI_DECLARE_EMITTER("{3518d488-5339-49a4-b927-bb69d7d9cf18}")
BOI_END_EMITTERS(TextComponent)


BOI_BEGIN_FUNCSETS(TextComponent)
    BOI_DECLARE_FUNCSET_1("{c768ae35-9c07-46d8-be25-3ad3f65ccdfd}",
                          BOI_FUNCSET_FUNC(TextComponent, GetText))
BOI_END_FUNCSETS(TextComponent)


BOI_BEGIN_CALLERS(TextComponent)
BOI_END_CALLERS(TextComponent)


TextComponent::TextComponent()
    : Component(BOI_STD_C(Text)),
      m_numTransactions(0),
      m_document(),
      m_cursor(&m_document),
      m_pLayout(NULL),
      m_handledPress(false),
      m_helper(this)
{
    m_pLayout = m_document.documentLayout();

    QObject::connect(m_pLayout, SIGNAL(update(const QRectF&)),
                     &m_helper, SLOT(Update(const QRectF&)),
                     Qt::DirectConnection);

    QObject::connect(m_pLayout, SIGNAL(documentSizeChanged(const QSizeF&)),
                     &m_helper, SLOT(DocSizeChanged(const QSizeF&)),
                     Qt::DirectConnection);
}


bool TextComponent::Initialize()
{
    m_document.setPlainText("New Text");
    return true;
}


void TextComponent::Destroy()
{
}


bool TextComponent::HandlesKeyEvents()
{
    return true;
}


void TextComponent::HandleKeyEvent(KeyEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == KeyEvent::Type_Press)
    {
        LockDraw();

        if (!m_handledPress)
        {
            m_cursor.select(QTextCursor::Document);
            m_cursor.removeSelectedText();

            m_handledPress = true;
        }

        switch (pEvent->key)
        {
            case Qt::Key_Backspace:
                m_cursor.deletePreviousChar();
                EmitText();
                break;

            case Qt::Key_Left:
                m_cursor.movePosition(QTextCursor::Left);
                Update(); // TODO: only update the part that needs updating?
                break;

            case Qt::Key_Right:
                m_cursor.movePosition(QTextCursor::Right);
                Update(); // TODO: only update the part that needs updating?
                break;

            case Qt::Key_Up:
                m_cursor.movePosition(QTextCursor::Up);
                Update(); // TODO: only update the part that needs updating?
                break;

            case Qt::Key_Down:
                m_cursor.movePosition(QTextCursor::Down);
                Update(); // TODO: only update the part that needs updating?
                break;

            case Qt::Key_Return:
                m_cursor.insertText("\n");
                EmitText();
                break;

            case Qt::Key_Shift:
            case Qt::Key_Control:
                /*
                 * Avoid handling the shift and control keys
                 * since they are incorrectly printed below.
                 */
                break;

            default:
                {
                    QChar ch(pEvent->key);

                    if (ch.isPrint())
                    {
                        if (pEvent->modifiers & KeyEvent::Modifier_Shift)
                        {
                            m_cursor.insertText(ch);
                        }
                        else
                        {
                            m_cursor.insertText(ch.toLower());
                        }

                        EmitText();
                    }
                }
                break;
        }

        UnlockDraw();
    }
}


void TextComponent::HandleEmitterConnected(int emitter,
                                           int componentId)
{
    Q_UNUSED(componentId);

    if (EmitterHasNew(emitter))
    {
        if (emitter == Emitter_Text)
        {
            EmitText();
        }
    }
}


int TextComponent::AcceptTransaction(int funcSet)
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


void TextComponent::TransactionEnded(int id)
{
    Q_UNUSED(id);

    m_numTransactions.fetchAndStoreRelaxed(0);
}


void TextComponent::Draw(QPainter* pPainter,
                         const QStyleOptionGraphicsItem* pOption)
{
    Q_UNUSED(pOption);

    LockDraw();

    QAbstractTextDocumentLayout::PaintContext context;
    context.cursorPosition = m_cursor.position();

    if (m_cursor.hasSelection())
    {
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = m_cursor;
        selection.format.setBackground(context.palette.brush(QPalette::Active, QPalette::Highlight));

        QVector<QAbstractTextDocumentLayout::Selection> selections;
        selections.append(selection);
        context.selections = selections;
    }

    m_pLayout->draw(pPainter, context);

    UnlockDraw();
}


void TextComponentHelper::Update(const QRectF& rect)
{
    m_pTextComponent->Update(rect);
}


void TextComponentHelper::DocSizeChanged(const QSizeF& newSize)
{
    QRectF rect(QPointF(0.0, 0.0), newSize);
    m_pTextComponent->SetBoundingRect(rect);
}


void TextComponent::EmitText()
{
    if (EmitterConnected(Emitter_Text))
    {
        DRef dref = SI()->NewData(BOI_STD_D(String));
        *dref.GetWriteInstance<QString>() = m_document.toPlainText();

        Emit(Emitter_Text, dref, true);
    }
}


void TextComponent::SetText(DRef& dref)
{
    if (dref.Type() == BOI_STD_D(String))
    {
        QString text = *dref.GetReadInstance<QString>();

        LockDraw();

        /*
         * NOTE: QTextDocument::setPlainText(...) can not
         * be used here because it fails trying to create
         * child QObjects of the QTextDocument due to the
         * fact that this method can be called from different
         * threads and thus the child QObject is created in
         * a different thread which is not possible when
         * parenting.
         */

        m_cursor.select(QTextCursor::Document);
        m_cursor.removeSelectedText();
        m_cursor.insertText(text);

        m_handledPress = true;

        UnlockDraw();

        Update();
    }
}


void TextComponent::SetText(DRef& dref, int source)
{
    Q_UNUSED(source);

    SetText(dref);
}


void TextComponent::AppendText(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(String))
    {
        QString text = *dref.GetReadInstance<QString>();

        LockDraw();

        /*
         * NOTE: QTextDocument::setPlainText(...) can not
         * be used here because it fails trying to create
         * child QObjects of the QTextDocument due to the
         * fact that this method can be called from different
         * threads and thus the child QObject is created in
         * a different thread which is not possible when
         * parenting.
         */

        m_cursor.clearSelection();
        m_cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        m_cursor.insertText(text);

        m_handledPress = true;

        UnlockDraw();

        Update();
    }
}


void TextComponent::InsertText(DRef& dref, int source)
{
    Q_UNUSED(source);

    if (dref.Type() == BOI_STD_D(String))
    {
        QString text = *dref.GetReadInstance<QString>();

        LockDraw();

        /*
         * NOTE: QTextDocument::setPlainText(...) can not
         * be used here because it fails trying to create
         * child QObjects of the QTextDocument due to the
         * fact that this method can be called from different
         * threads and thus the child QObject is created in
         * a different thread which is not possible when
         * parenting.
         */

        m_cursor.insertText(text);

        m_handledPress = true;

        UnlockDraw();

        Update();
    }
}


void TextComponent::SetFontSize(DRef& dref, int source)
{
    Q_UNUSED(source);

    double newSize;
    bool doSetSize = false;
    int drefType = dref.Type();

    if (drefType == BOI_STD_D(Double))
    {
        newSize = *dref.GetReadInstance<double>();

        doSetSize = true;
    }
    else if (drefType == BOI_STD_D(Float))
    {
        newSize = *dref.GetReadInstance<float>();

        doSetSize = true;
    }
    else if (drefType == BOI_STD_D(Int))
    {
        newSize = *dref.GetReadInstance<int>();

        doSetSize = true;
    }
    else if (drefType == BOI_STD_D(Font))
    {
        const QFont font = *dref.GetReadInstance<QFont>();
        newSize = font.pointSizeF();

        doSetSize = true;
    }
    else if (drefType == BOI_STD_D(String))
    {
        bool conversionValid;
        const QString* pString = dref.GetReadInstance<QString>();

        newSize = pString->toDouble(&conversionValid);

        if (conversionValid)
        {
            doSetSize = true;
        }
    }


    if (doSetSize)
    {
        LockDraw();

        QTextCharFormat charFormat = m_cursor.charFormat();
        charFormat.setFontPointSize(newSize);

        m_document.setDefaultFont(charFormat.font());

        int position = m_cursor.position();

        m_cursor.select(QTextCursor::Document);
        m_cursor.setCharFormat(charFormat);
        m_cursor.clearSelection();

        m_cursor.setPosition(position);

        UnlockDraw();

        Update();
    }
}


DRef TextComponent::GetText(int id, DRef& dref)
{
    Q_UNUSED(id);
    Q_UNUSED(dref);

    DRef stringRef = SI()->NewData(BOI_STD_D(String));

    LockDraw();

    *stringRef.GetWriteInstance<QString>() = m_document.toPlainText();

    UnlockDraw();

    return stringRef;
}


void TextComponent::Import(const QHash<int, DRef>& in)
{
    DRef dref = in.value(1);
    SetText(dref);

    dref = in.value(2);
    SetFontSize(dref, -1);
}


void TextComponent::Export(QHash<int, DRef>& out)
{
    DRef textRef = SI()->NewData(BOI_STD_D(String));
    DRef fontRef = SI()->NewData(BOI_STD_D(Font));

    LockDraw();

    *textRef.GetWriteInstance<QString>() = m_document.toPlainText();
    *fontRef.GetWriteInstance<QFont>() = m_cursor.charFormat().font();

    UnlockDraw();

    out.insert(1, textRef);
    out.insert(2, fontRef);
}


} // namespace BOI

