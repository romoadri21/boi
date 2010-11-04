/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SCENELAYERID_H
#define __BOI_SCENELAYERID_H


namespace BOI {


enum SceneLayerId
{
    SceneLayerId_None     = 0x0,
    SceneLayerId_Main     = 0x1,
    SceneLayerId_System   = 0x2,
    SceneLayerId_Overlay  = 0x4,
    SceneLayerId_Underlay = 0x8,

    SceneLayerId_All = SceneLayerId_Main |
                       SceneLayerId_System |
                       SceneLayerId_Overlay |
                       SceneLayerId_Underlay
};


} // namespace BOI


#endif //__BOI_SCENELAYERID_H

