/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_KEYEVENTLISTENER_H
#define __BOI_KEYEVENTLISTENER_H


namespace BOI {


class KeyEvent;


class KeyEventListener
{
    public:
        virtual void HandleKeyEvent(KeyEvent* pEvent) = 0;
};


} // namespace BOI


#endif //__BOI_KEYEVENTLISTENER_H

