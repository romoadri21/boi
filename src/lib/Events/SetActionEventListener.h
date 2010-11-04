/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SETACTIONEVENTLISTENER_H
#define __BOI_SETACTIONEVENTLISTENER_H


namespace BOI {


class SetActionEvent;


class SetActionEventListener
{
    public:
        virtual void HandleSetActionEvent(SetActionEvent* pEvent) = 0;
};


} // namespace BOI


#endif //__BOI_SETACTIONEVENTLISTENER_H

