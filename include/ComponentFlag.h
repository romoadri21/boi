/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COMPONENTFLAG_H
#define __BOI_COMPONENTFLAG_H


namespace BOI {


enum ComponentFlag
{
    ComponentFlag_HandlesTouchEvents = 0x0001,
    ComponentFlag_HandlesKeyEvents   = 0x0002,
    ComponentFlag_ClipsChildren      = 0x0004,
    ComponentFlag_IsSelectable       = 0x0008,
    ComponentFlag_HasCustomHitTest   = 0x0010,

    NumComponentFlags
};


} // namespace BOI


#endif //__BOI_COMPONENTFLAG_H

