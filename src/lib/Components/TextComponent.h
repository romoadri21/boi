/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TEXTCOMPONENT_H
#define __BOI_TEXTCOMPONENT_H


#include <QObject>
#include <QAtomicInt>
#include <QTextCursor>
#include <QTextDocument>
#include "StandardEmitters.h"
#include "Component.h"


namespace BOI {


class TextComponent;


/*
 * A helper class which is derived from
 * QObject that can directly receive and
 * forward the signals coming from other
 * QObjects.
 */

class TextComponentHelper
    : public QObject
{
    Q_OBJECT

    public:
        TextComponentHelper(TextComponent* pComponent)
            : m_pTextComponent(pComponent)
        {
        }

    public slots:
        void Update(const QRectF& rect);
        void DocSizeChanged(const QSizeF& newSize);

    private:
        TextComponent* m_pTextComponent;
};


class TextComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    BOI_DEFINE_RECEIVER_FUNC(TextComponent, SetText);
    BOI_DEFINE_RECEIVER_FUNC(TextComponent, AppendText);
    BOI_DEFINE_RECEIVER_FUNC(TextComponent, InsertText);

    BOI_DEFINE_RECEIVER_FUNC(TextComponent, SetFontSize);

    BOI_DEFINE_FUNCSET_FUNC(TextComponent, GetText);

    public:
        TextComponent();

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual bool HandlesKeyEvents();
        virtual void HandleKeyEvent(KeyEvent* pEvent);

        virtual void HandleEmitterConnected(int emitter,
                                            int componentId);

        virtual int AcceptTransaction(int funcSet);
        virtual void TransactionEnded(int id);

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

        virtual void Import(const QHash<int, DRef>& in);
        virtual void Export(QHash<int, DRef>& out);

    private:
        enum
        {
            Emitter_Text = StandardEmitters::NumEmitters
        };

    private:
        void EmitText();
        void SetText(DRef& dref);

    private:
        QAtomicInt m_numTransactions;

        QTextDocument m_document;
        QTextCursor   m_cursor;
        QAbstractTextDocumentLayout* m_pLayout;

        bool m_handledPress;

        friend class TextComponentHelper;
        TextComponentHelper m_helper;
};


} // namespace BOI


#endif //__BOI_TEXTCOMPONENT_H

