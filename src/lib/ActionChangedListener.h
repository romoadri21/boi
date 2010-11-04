/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ACTIONCHANGEDLISTENER_H
#define __BOI_ACTIONCHANGEDLISTENER_H


namespace BOI {


/*
 * The interface for objects that get
 * notified when the active action has
 * changed in the ActionEngine.
 */

class ActionChangedListener
{
    public:
        virtual void HandleActionChanged() = 0;
};


} // namespace BOI


#endif //__BOI_ACTIONCHANGEDLISTENER_H

