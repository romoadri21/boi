/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "StandardReceivers.h"


namespace BOI {


const char* StandardReceivers::s_uuids[] = 
{
    "{84e256a6-9291-451b-a0ff-f38602d2f839}", // Receiver_SetPosition
    "{af919167-3fd8-4fad-8990-cce279a8366c}", // Receiver_SetRotation
    "{0107f9b1-af4c-4b48-b772-c21c567cb53a}", // Receiver_SetOpacity
    "{f4e6a67a-8cd5-441c-a7c0-367c8360a146}", // Receiver_SetVisible
};


const char* StandardReceivers::Uuid(int type)
{
    return s_uuids[type];
}


} // namespace BOI

