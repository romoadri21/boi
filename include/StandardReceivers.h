/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STANDARDRECEIVERS_H
#define __BOI_STANDARDRECEIVERS_H


#include "BOILib.h"


#define BOI_STD_R(receiver)  (BOI::StandardReceivers::Receiver_##receiver)
#define BOI_UUID_R(receiver) (BOI::StandardReceivers::Uuid(BOI::StandardReceivers::Receiver_##receiver))


namespace BOI {


/*
 * Identifiers for the built-in component receivers.
 */

class BOI_LIB_EXPORT StandardReceivers
{
    public:
        enum
        {
            Receiver_SetPosition =  0,
            Receiver_SetRotation,
            Receiver_SetOpacity,
            Receiver_SetVisible,

            NumReceivers
        };

    public:
        static const char* Uuid(int type);

    private:
        static const char* s_uuids[];
};


} // namespace BOI


#endif //__BOI_STANDARDRECEIVERS_H

