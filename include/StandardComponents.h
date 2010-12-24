/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STANDARDCOMPONENTS_H
#define __BOI_STANDARDCOMPONENTS_H


#include "BOILib.h"


#define BOI_STD_C(component)  (BOI::StandardComponents::Component_##component)
#define BOI_UUID_C(component) (BOI::StandardComponents::Uuid(BOI::StandardComponents::Component_##component))


namespace BOI {


/*
 * Identifiers for the built-in components.
 */

class BOI_LIB_EXPORT StandardComponents
{
    public:
        enum
        {
            Component_Invalid = -1,
            Component_Menu = 0,
            Component_XGesture,
            Component_WindowSize,
            Component_VerticalLayout,
            Component_HorizontalLayout,
            Component_Text,
            Component_TextInput,
            Component_Timer,
            Component_Browser,
            Component_Javascript,

            NumComponents
        };

    public:
        static const char* Uuid(int type);

    private:
        static const char* s_uuids[];

};


} // namespace BOI


#endif //__BOI_STANDARDCOMPONENTS_H

