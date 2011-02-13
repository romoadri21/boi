/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "Action.h"
#include "StandardActions.h"
#include "Actions/NullAction.h"
#include "Actions/XGestureAction.h"
#include "Actions/PanAction.h"
#include "Actions/ZoomAction.h"
#include "Actions/MenuAction.h"
#include "Actions/MoveAction.h"
#include "Actions/CopyAction.h"
#include "Actions/PasteAction.h"
#include "Actions/InsertAction.h"
#include "Actions/DeleteAction.h"
#include "Actions/ResizeAction.h"
#include "Actions/ConnectAction.h"
#include "Actions/CollectAction.h"
#include "Actions/LoadUrlAction.h"
#include "Actions/PutTextAction.h"
#include "Actions/SetTextAction.h"
#include "Actions/ShutdownAction.h"
#include "Actions/ResetViewAction.h"
#include "Actions/SetChildAction.h"
#include "Actions/SetNumberAction.h"
#include "Actions/RectSelectAction.h"
#include "Actions/ZoomToRectAction.h"
#include "Actions/InsertTextAction.h"
#include "Actions/AppendTextAction.h"
#include "Actions/FreeRotateAction.h"
#include "Actions/SetRotationAction.h"
#include "Actions/MoveToLayerAction.h"
#include "Actions/AddToLayoutAction.h"
#include "Actions/CaptureViewAction.h"
#include "Actions/FitAllInViewAction.h"
#include "Actions/InsertBrowserAction.h"
#include "Actions/ToggleHotSpotAction.h"
#include "Actions/ResetViewScaleAction.h"
#include "Actions/ClearSelectionAction.h"
#include "Actions/InsertCollectionAction.h"
#include "Actions/RemoveFromLayoutAction.h"
#include "Actions/ToggleFullScreenAction.h"
#include "TypeRegistrar.h"
#include "Factories/CoreActionsFactory.h"


namespace BOI {


CoreActionsFactory::CoreActionsFactory()
    : Factory()
{
}


void CoreActionsFactory::RegisterTypes(TypeRegistrar* pRegistrar)
{
    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Null)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(XGesture)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Menu)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Pan)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Zoom)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(ToggleFullScreen)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Insert)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(ResetView)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Move)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(SetChild)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(AddToLayout)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(RemoveFromLayout)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(FreeRotate)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(SetRotation)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(ResetViewScale)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Resize)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Shutdown)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(RectSelect)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(InsertText)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Collect)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(InsertCollection)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Delete)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Connect)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(LoadUrl)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(AppendText)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(PutText)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(ClearSelection)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(FitAllInView)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Copy)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(Paste)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(ToggleHotSpot)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(InsertBrowser)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(SetNumber)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(ZoomToRect)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(MoveToLayer)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(CaptureView)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreActionsFactory, GetAction),
                             NULL,
                             StandardActions::Uuid(BOI_STD_A(SetText)));
}


Object* CoreActionsFactory::GetAction(int type)
{
    Action* pAction = NULL;

    switch (type)
    {
        case BOI_STD_A(Null):
            pAction = new NullAction();
            break;

        case BOI_STD_A(XGesture):
            pAction = new XGestureAction();
            break;

        case BOI_STD_A(Menu):
            pAction = new MenuAction();
            break;

        case BOI_STD_A(Pan):
            pAction = new PanAction();
            break;

        case BOI_STD_A(Zoom):
            pAction = new ZoomAction();
            break;

        case BOI_STD_A(ToggleFullScreen):
            pAction = new ToggleFullScreenAction();
            break;

        case BOI_STD_A(Insert):
            pAction = new InsertAction();
            break;

        case BOI_STD_A(ResetView):
            pAction = new ResetViewAction();
            break;

        case BOI_STD_A(Move):
            pAction = new MoveAction();
            break;

        case BOI_STD_A(SetChild):
            pAction = new SetChildAction();
            break;

        case BOI_STD_A(AddToLayout):
            pAction = new AddToLayoutAction();
            break;

        case BOI_STD_A(RemoveFromLayout):
            pAction = new RemoveFromLayoutAction();
            break;

        case BOI_STD_A(FreeRotate):
            pAction = new FreeRotateAction();
            break;

        case BOI_STD_A(SetRotation):
            pAction = new SetRotationAction();
            break;

        case BOI_STD_A(ResetViewScale):
            pAction = new ResetViewScaleAction();
            break;

        case BOI_STD_A(Resize):
            pAction = new ResizeAction();
            break;

        case BOI_STD_A(Shutdown):
            pAction = new ShutdownAction();
            break;

        case BOI_STD_A(RectSelect):
            pAction = new RectSelectAction();
            break;

        case BOI_STD_A(InsertText):
            pAction = new InsertTextAction();
            break;

        case BOI_STD_A(Collect):
            pAction = new CollectAction();
            break;

        case BOI_STD_A(InsertCollection):
            pAction = new InsertCollectionAction();
            break;

        case BOI_STD_A(Delete):
            pAction = new DeleteAction();
            break;

        case BOI_STD_A(Connect):
            pAction = new ConnectAction();
            break;

        case BOI_STD_A(LoadUrl):
            pAction = new LoadUrlAction();
            break;

        case BOI_STD_A(AppendText):
            pAction = new AppendTextAction();
            break;

        case BOI_STD_A(PutText):
            pAction = new PutTextAction();
            break;

        case BOI_STD_A(ClearSelection):
            pAction = new ClearSelectionAction();
            break;

        case BOI_STD_A(FitAllInView):
            pAction = new FitAllInViewAction();
            break;

        case BOI_STD_A(Copy):
            pAction = new CopyAction();
            break;

        case BOI_STD_A(Paste):
            pAction = new PasteAction();
            break;

        case BOI_STD_A(ToggleHotSpot):
            pAction = new ToggleHotSpotAction();
            break;

        case BOI_STD_A(InsertBrowser):
            pAction = new InsertBrowserAction();
            break;

        case BOI_STD_A(SetNumber):
            pAction = new SetNumberAction();
            break;

        case BOI_STD_A(ZoomToRect):
            pAction = new ZoomToRectAction();
            break;

        case BOI_STD_A(MoveToLayer):
            pAction = new MoveToLayerAction();
            break;

        case BOI_STD_A(CaptureView):
            pAction = new CaptureViewAction();
            break;

        case BOI_STD_A(SetText):
            pAction = new SetTextAction();
            break;

        default:
            break;
    }

    return pAction; 
}


} // namespace BOI

