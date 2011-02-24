/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STANDARDACTIONS_H
#define __BOI_STANDARDACTIONS_H


#include "BOILib.h"


#define BOI_STD_A(action)  (BOI::StandardActions::Action_##action)
#define BOI_UUID_A(action) (BOI::StandardActions::Uuid(BOI::StandardActions::Action_##action))


namespace BOI {


/*
 * Identifiers for the built-in actions.
 */

class BOI_LIB_EXPORT StandardActions
{
    public:
        enum
        {
            Action_Null = 0,
            Action_Gesture,
            Action_Menu,
            Action_Pan,
            Action_Zoom,
            Action_ToggleFullScreen,
            Action_Insert,
            Action_ResetView,
            Action_Move,
            Action_SetChild,
            Action_AddToLayout,
            Action_RemoveFromLayout,
            Action_FreeRotate,
            Action_SetRotation,
            Action_ResetViewScale,
            Action_Resize,
            Action_Shutdown,
            Action_RectSelect,
            Action_InsertText,
            Action_Collect,
            Action_InsertCollection,
            Action_Delete,
            Action_Connect,
            Action_LoadUrl,
            Action_AppendText,
            Action_PutText,
            Action_ClearSelection,
            Action_FitAllInView,
            Action_Copy,
            Action_Paste,
            Action_ToggleHotSpot,
            Action_InsertBrowser,
            Action_SetNumber,
            Action_ZoomToRect,
            Action_MoveToLayer,
            Action_CaptureView,
            Action_SetText,
            Action_ScrollBox,
            Action_TextInput,

            NumActions
        };

    public:
        static const char* Uuid(int type);

    private:
        static const char* s_uuids[];

};


} // namespace BOI


#endif //__BOI_STANDARDACTIONS_H

