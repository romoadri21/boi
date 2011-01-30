/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "StandardMenus.h"


namespace BOI {


const char* StandardMenus::s_uuids[] = 
{
    "{e76dd871-9efb-4abe-bc93-2cc6af7dac20}", // Menu_Root
    "{dfe3e724-fb4b-42cc-9140-b97cc44e7dd5}", // Menu_Component
    "{8a8ac1c4-d484-4530-a009-55c64f2beb16}", // Menu_Resize
    "{2bf99e92-61c6-4ad4-84cb-8eb8824924a3}", // Menu_Layout
    "{7fceed55-47bd-42ce-9cf0-c91f2f9bb867}", // Menu_Browser
    "{838e233a-1083-4b6e-b20d-89fb3f0d405f}", // Menu_Text
    "{f559c728-f7aa-4c35-a800-591dc6fe8c59}", // Menu_Border
};


const char* StandardMenus::Uuid(int type)
{
    return s_uuids[type];
}


} // namespace BOI

