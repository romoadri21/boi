/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STANDARDDATATYPES_H
#define __BOI_STANDARDDATATYPES_H


#include "BOILib.h"


#define BOI_STD_D_VERSION 1

#define BOI_STD_D(datatype)  (BOI::StandardDataTypes::DataType_##datatype)
#define BOI_UUID_D(datatype) (BOI::StandardDataTypes::Uuid(BOI::StandardDataTypes::DataType_##datatype))


namespace BOI {


/*
 * Identifiers for the built-in data types.
 */

class BOI_LIB_EXPORT StandardDataTypes
{
    public:
        enum
        {
            DataType_Invalid = -1,
            DataType_Int = 0,
            DataType_Char,
            DataType_Bool,
            DataType_Short,
            DataType_Float,
            DataType_Double,
            DataType_String,
            DataType_Size,
            DataType_Color,
            DataType_Brush,
            DataType_Font,
            DataType_Pen,
            DataType_Point,
            DataType_MenuSet,
            DataType_Rect,
            DataType_BoundingBox,
            DataType_Image,
            DataType_IntList,

            NumTypes
        };

    public:
        static const char* Uuid(int type);

    private:
        static const char* s_uuids[];
};


} // namespace BOI


#endif //__BOI_STANDARDDATATYPES_H

