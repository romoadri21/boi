/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TEXTINPUTCOMPONENT_H
#define __BOI_TEXTINPUTCOMPONENT_H


#include <QPen>
#include <QFont>
#include <QRectF>
#include <QSizeF>
#include <QBrush>
#include <QString>
#include <QAtomicInt>
#include "StandardEmitters.h"
#include "Component.h"


namespace BOI {


class ActionArgs;


class TextInputComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    BOI_DEFINE_RECEIVER_FUNC(TextInputComponent, SetText);
    BOI_DEFINE_RECEIVER_FUNC(TextInputComponent, ClearOnNextPress);

    BOI_DEFINE_FUNCSET_FUNC(TextInputComponent, SetAction);
    BOI_DEFINE_FUNCSET_FUNC(TextInputComponent, SetActionArgs);

    public:
        TextInputComponent();

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual void HandleStateChanged(StateId stateId, DRef& dref);

        virtual bool HandlesKeyEvents();
        virtual void HandleKeyEvent(KeyEvent* pEvent);

        virtual int AcceptTransaction(int funcSet);
        virtual void TransactionEnded(int id);

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

    private:
        enum
        {
            Emitter_Text = StandardEmitters::NumEmitters
        };

    private:
        void EmitText();
        void UpdateDimensions();

    private:
        QString m_text;

        bool m_clearOnNext;
        bool m_shiftPressed;

        QSizeF m_windowSize;
        QRectF m_boundingRect;
        QRectF m_innerBoundingRect;

        QPen m_textPen;
        QBrush m_bgFill;
        QBrush m_textBgFill;

        QFont m_font;

        int m_action;
        ActionArgs* m_pActionArgs;

        QAtomicInt m_numTransactions;
};


} // namespace BOI


#endif //__BOI_TEXTINPUTCOMPONENT_H

