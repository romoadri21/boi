/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_UUIDTYPE_H
#define __BOI_UUIDTYPE_H


#include <QString>
#include "Utilities/Tuple.h"


namespace BOI {


/*
 * Enum describing the type that a uuid
 * represents. For instance, a uuid of
 * type UuidType_Action is a uuid that
 * uniquely refers to an Action (an Action
 * uuid; usually registered by an action
 * Factory with the system during plugin
 * initialization).
 */


enum UuidType
{
    UuidType_Invalid    = 0x0,
    UuidType_Data       = 0x1,
    UuidType_Action     = 0x2,
    UuidType_Component  = 0x4,
    UuidType_Collection = 0x8,

    UuidType_All = UuidType_Data |
                   UuidType_Action |
                   UuidType_Component |
                   UuidType_Collection
};


typedef Tuple2<QString, UuidType> TypedUuid;


} // namespace BOI


#endif //__BOI_UUIDTYPE_H

