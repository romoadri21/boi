/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "StandardActions.h"


namespace BOI {


const char* StandardActions::s_uuids[] = 
{
    "{0bd92f90-6822-4286-a040-796550328850}", // Action_Null
    "{e7d9d4b1-c0a1-48bd-8aed-bad7f6bfc893}", // Action_Gesture
    "{eb5fed2b-2532-4979-bb1f-bb60f19e377c}", // Action_Menu
    "{9c21c94b-01d7-4601-8856-859a8c79a5c0}", // Action_Pan
    "{957c9060-86ce-493f-83f2-c945f565e0fa}", // Action_Zoom
    "{0b7e24b1-050c-4e0a-895f-65362105fc39}", // Action_ToggleFullScreen
    "{fe85b65a-8df8-4fd5-b507-c17563b78f2a}", // Action_Insert
    "{73bec14c-c0e3-4af7-9d56-62a5830d880c}", // Action_ResetView
    "{c31d1d3b-9a85-48b8-9c81-1cd8b73e1b48}", // Action_Move
    "{0b922579-f7ab-4aab-8a5d-49218ccccc59}", // Action_SetChild
    "{0bad5748-500b-41c3-8b0a-483368e64637}", // Action_AddToLayout
    "{c16d4bcf-9b08-481e-9c14-22e8f41db66d}", // Action_RemoveFromLayout
    "{cecc0547-c3cb-4e70-9f9c-129ecba4537c}", // Action_FreeRotate
    "{5127a176-b547-4e06-9dde-be122d9a08cf}", // Action_SetRotation
    "{aa2d5c20-410a-41a6-a476-567481df1024}", // Action_ResetViewScale
    "{97c827df-42eb-4ebd-bc37-ebbb862289bd}", // Action_Resize
    "{540c31dd-72a6-4270-8364-2442b1bebc4f}", // Action_Shutdown
    "{bc09fbe7-085c-4052-8a04-8e8aeaaefabd}", // Action_RectSelect
    "{465f7acb-d761-4162-bba7-758215aa7a94}", // Action_InsertText
    "{b949b758-ae06-46a2-88e5-f440d9b2b2fb}", // Action_Collect
    "{32e0a0ac-2507-40fd-9d1b-d7ca6588d86c}", // Action_InsertCollection
    "{551933c7-6c72-4810-b8c6-6435dc392172}", // Action_Delete
    "{6901e1c6-d22c-4673-a900-6a341f059d95}", // Action_Connect
    "{a8be4b35-1275-4183-862f-7a22a84545f5}", // Action_LoadUrl
    "{72f198dc-9148-47b1-b5fa-863c84293f15}", // Action_AppendText
    "{855206cd-4b7c-4ed0-995c-cb70c083742d}", // Action_PutText
    "{3831d0cf-345b-4f68-a59a-f74ee1d2ae4d}", // Action_ClearSelection
    "{acad916e-1a54-4409-ac04-d611aa67d971}", // Action_FitAllInView
    "{cc5ae59e-b61e-42db-9b90-cd03ccd59a00}", // Action_Copy
    "{29d681a6-8c1c-4d35-92fd-5fa6928dc56c}", // Action_Paste
    "{31dc8fc7-1c4b-4510-af87-3ef2074f1a15}", // Action_ToggleHotSpot
    "{b4388785-830d-4f4d-b1a4-3b95a5737628}", // Action_InsertBrowser
    "{4849a9fd-1324-49ec-8952-a1b4d3b05510}", // Action_SetNumber
    "{ae5ea269-83e9-4718-8391-d285c7a44ef5}", // Action_ZoomToRect
    "{326f0aa5-43f8-4030-9ab4-9b1239bf0248}", // Action_MoveToLayer
    "{8c7ce920-430d-4464-9ff4-ad5e62d783aa}", // Action_CaptureView
    "{316e6510-7d8c-47de-9ed6-727817f281d6}", // Action_SetText
    "{a140412d-297c-4f72-808c-5735fd18f915}", // Action_ScrollBox
    "{f41d88dc-37f1-4106-b89e-98bb8687ae97}", // Action_TextInput
};


const char* StandardActions::Uuid(int type)
{
    return s_uuids[type];
}


} // namespace BOI

