/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STANDARDMENUS_H
#define __BOI_STANDARDMENUS_H


#include "BOILib.h"


#define BOI_STD_M(menu)  (BOI::StandardMenus::Menu_##menu)
#define BOI_UUID_M(menu) (BOI::StandardMenus::Uuid(BOI::StandardMenus::Menu_##menu))


namespace BOI {


/*
 * Identifiers for the built-in menus.
 */

class BOI_LIB_EXPORT StandardMenus
{
    public:
        enum
        {
            Menu_Root =  0,
            Menu_Component,
            Menu_Resize,
            Menu_Layout,
            Menu_Browser,
            Menu_Text,
            Menu_Border,
            Menu_Font,

            NumMenus
        };

    public:
        static const char* Uuid(int type);

    private:
        static const char* s_uuids[];
};


} // namespace BOI


#endif //__BOI_STANDARDMENUS_H

