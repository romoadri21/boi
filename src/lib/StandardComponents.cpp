/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "StandardComponents.h"


namespace BOI {


const char* StandardComponents::s_uuids[] = 
{
    "{e03f5cd5-7ecd-485a-95c8-9ce2f85f7b7d}", // Component_Menu
    "{38b601c7-d72d-41dc-b7f2-d8b737844d75}", // Component_XGesture
    "{ab1ac59e-050c-475c-8da8-729da945c834}", // Component_WindowSize
    "{27402cb9-3137-4219-b264-cace99a2f1fa}", // Component_VerticalLayout
    "{d2032b64-1e16-4638-ab45-5aa95fcb5070}", // Component_HorizontalLayout
    "{d810a602-5fc4-4bac-a39f-e68b8ef32909}", // Component_Text
    "{6ef865a1-563e-4281-9f3c-076b311794af}", // Component_TextInput
    "{628b0c5a-11b4-443b-8391-3c7b935e7dcc}", // Component_Timer
    "{1c2a1bf2-05ae-4198-9847-6ba2053de8e8}", // Component_Browser
    "{e895b361-2015-4301-8328-f5e3780a479e}", // Component_Javascript
};


const char* StandardComponents::Uuid(int type)
{
    return s_uuids[type];
}


} // namespace BOI

