/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_KEYEVENT_H
#define __BOI_KEYEVENT_H


namespace BOI {


class KeyEvent
{
    public:
        enum
        {
            Type_Press = 0,
            Type_Release
        };

        enum
        {
            Modifier_None = 0x0,
            Modifier_Shift = 0x1,
            Modifier_Control = 0x2
        };

    public:
        int id;
        int key;
        int type;
        int modifiers;
};


} // namespace BOI


#endif //__BOI_KEYEVENT_H

