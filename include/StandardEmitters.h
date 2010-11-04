/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STANDARDEMITTERS_H
#define __BOI_STANDARDEMITTERS_H


#include "BOILib.h"


#define BOI_STD_E(emitter)  (BOI::StandardEmitters::Emitter_##emitter)
#define BOI_UUID_E(emitter) (BOI::StandardEmitters::Uuid(BOI::StandardEmitters::Emitter_##emitter))


namespace BOI {


/*
 * Identifiers for the built-in component emitters.
 */

class BOI_LIB_EXPORT StandardEmitters
{
    public:
        enum
        {
            Emitter_ParentBoundingBox = 0,

            NumEmitters
        };

    public:
        static const char* Uuid(int type);

    private:
        static const char* s_uuids[];
};


} // namespace BOI


#endif //__BOI_STANDARDEMITTERS_H

