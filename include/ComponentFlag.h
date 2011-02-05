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
    ComponentFlag_HandlesTouchEvents = 0x1,
    ComponentFlag_HandlesKeyEvents   = 0x2,
    ComponentFlag_ClipsChildren      = 0x4,
    ComponentFlag_IsSelectable       = 0x8,

    NumComponentFlags
};


} // namespace BOI


#endif //__BOI_COMPONENTFLAG_H

