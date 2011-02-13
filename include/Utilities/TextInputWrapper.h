/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TEXTINPUTWRAPPER_H
#define __BOI_TEXTINPUTWRAPPER_H


#include "CRef.h"
#include "BOILib.h"


class QString;


namespace BOI {


class ASI;


class BOI_LIB_EXPORT TextInputWrapper
{
    public:
        TextInputWrapper();

        bool Initialize(ASI* pSI);
        void Destroy();

        void SetAction(ASI* pSI, int actionId);
        void SetText(ASI* pSI, const QString& text);
        void SetClearOnNextPress(ASI* pSI, bool clear);

        void Show(ASI* pSI);
        void Hide(ASI* pSI);

        bool IsValid();
        bool IsVisible();

        void SetFocus(ASI* pSI);

    private:
        CRef m_cref;

        int m_funcSet;
        int m_setTextReceiver;
        int m_clearOnNextReceiver;

        bool m_visible;
};


} // namespace BOI


#endif //__BOI_TEXTINPUTWRAPPER_H

