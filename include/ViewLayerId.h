/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_VIEWLAYERID_H
#define __BOI_VIEWLAYERID_H


namespace BOI {


enum ViewLayerId
{
    ViewLayerId_Null     = 0x0,
    ViewLayerId_Main     = 0x1,
    ViewLayerId_System   = 0x2,
    ViewLayerId_Overlay  = 0x4,
    ViewLayerId_Underlay = 0x8,

    ViewLayerId_All = ViewLayerId_Main |
                      ViewLayerId_System |
                      ViewLayerId_Overlay |
                      ViewLayerId_Underlay
};


} // namespace BOI


#endif //__BOI_VIEWLAYERID_H

