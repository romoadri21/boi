/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "StandardDataTypes.h"


namespace BOI {


const char* StandardDataTypes::s_uuids[] = 
{
    "{2e73b84d-1f62-437a-ab49-76c47fb3e84e}", // DataType_Int
    "{683157f0-939f-40d5-8783-906e8376fd87}", // DataType_Char
    "{a748e966-f9c3-4689-8944-1051edf0d208}", // DataType_Bool
    "{6eeb93e7-06d8-46f2-9461-a3dabe0e20f8}", // DataType_Short
    "{e1653db4-2a90-4dbf-ba4d-c46ea7c5c513}", // DataType_Float
    "{af5bf91d-9472-4626-bb88-620e29c24c22}", // DataType_Double
    "{d5debd52-9287-4649-9803-4753493c7fac}", // DataType_String
    "{988c08a7-3823-47d5-bc1d-a4be987fb5e5}", // DataType_Size
    "{9ae22192-f4dd-4e89-bc8a-c7b19cc4f380}", // DataType_Color
    "{e967ce7d-81bf-4bc7-b0fd-0fb4fb3b8f97}", // DataType_Brush
    "{f19b3dc0-97cb-4451-b54a-b8f92365f685}", // DataType_Font
    "{03bf68f5-2c5a-45f5-bfc2-c8e3b676d560}", // DataType_Pen
    "{9defaaff-bd29-491c-94ee-25a234687cb3}", // DataType_Point
    "{e7a1534c-fd55-4ebb-8a0f-b2f96a1f3c75}", // DataType_MenuSet
    "{c0a06862-8bd2-4ddd-8a6e-7a618226d4c2}", // DataType_Rect
    "{cf02f714-cedf-41bb-a48b-917c2028fad6}", // DataType_BoundingBox
    "{e75c7728-47ed-49db-b016-66ff940a3ecf}", // DataType_Image
};


const char* StandardDataTypes::Uuid(int type)
{
    return s_uuids[type];
}


} // namespace BOI

