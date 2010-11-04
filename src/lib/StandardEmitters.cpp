/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "StandardEmitters.h"


namespace BOI {


const char* StandardEmitters::s_uuids[] = 
{
    "{6fbe5587-9110-4b87-9049-2d5058d09304}", // Emitter_ParentBoundingBox
};


const char* StandardEmitters::Uuid(int type)
{
    return s_uuids[type];
}


} // namespace BOI

