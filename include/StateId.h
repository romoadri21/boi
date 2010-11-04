/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STATEID_H
#define __BOI_STATEID_H


namespace BOI {


enum StateId
{
    StateId_Invalid = -1,
    StateId_InnerViewXMargin = 0,
    StateId_InnerViewYMargin,
    StateId_InnerViewRect,
    StateId_WindowSize,
    StateId_FgColor,
    StateId_BgColor,
    StateId_Brush,
    StateId_Font,
    StateId_Pen,
    StateId_ScrollVelocity,
    StateId_KineticScrollAcceleration,
    StateId_KineticScrollActivateVelocity,

    NumStateIds
};


} // namespace BOI


#endif //__BOI_STATEID_H

