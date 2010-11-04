/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_GUIREQUESTEVENTLISTENER_H
#define __BOI_GUIREQUESTEVENTLISTENER_H


namespace BOI {


class GuiRequestEventListener
{
    public:
        virtual void HandleGuiRequestEvent() = 0;
};


} // namespace BOI


#endif //__BOI_GUIREQUESTEVENTLISTENER_H

