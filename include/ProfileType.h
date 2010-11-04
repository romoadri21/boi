/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_PROFILETYPE_H
#define __BOI_PROFILETYPE_H


namespace BOI {


enum ProfileType
{
    ProfileType_Invalid   = 0x0,
    ProfileType_Emitters  = 0x1,
    ProfileType_Receivers = 0x2,
    ProfileType_Callers   = 0x4,
    ProfileType_FuncSets  = 0x8,

    ProfileType_Input = ProfileType_Receivers |
                        ProfileType_FuncSets,

    ProfileType_Output = ProfileType_Emitters |
                         ProfileType_Callers,

    ProfileType_Full = ProfileType_Emitters |
                       ProfileType_Receivers |
                       ProfileType_Callers |
                       ProfileType_FuncSets
};


} // namespace BOI


#endif //__BOI_PROFILETYPE_H

