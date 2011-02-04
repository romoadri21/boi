######################################################################
# BOI Library QMake build file
######################################################################

BOI_BASE_DIR = ../..

BOI_BIN_DIR      = $${BOI_BASE_DIR}/bin
BOI_LIB_DIR      = $${BOI_BASE_DIR}/lib
BOI_INCLUDE_DIR  = $${BOI_BASE_DIR}/include
BOI_RESOURCE_DIR = $${BOI_BASE_DIR}/resources

BOI_LIB_SRC_DIR = $${BOI_BASE_DIR}/src/lib

include($${BOI_BASE_DIR}/build/version.pri)
unix:VERSION = $${BOI_LIB_VERSION}

QT += svg sql webkit script

TEMPLATE    = lib
TARGET      = BOI
CONFIG     += dll debug
DESTDIR     = $${BOI_LIB_DIR}
DLLDESTDIR  = $${BOI_BIN_DIR}
OBJECTS_DIR = .
MOC_DIR     = .
RCC_DIR     = .

DEPENDPATH  += $${BOI_INCLUDE_DIR} $${BOI_LIB_SRC_DIR}
INCLUDEPATH += $${BOI_INCLUDE_DIR} $${BOI_LIB_SRC_DIR}

DEFINES += BOI_LIB

RESOURCES = $${BOI_RESOURCE_DIR}/BOI.qrc

HEADERS += \
    $${BOI_INCLUDE_DIR}/ASI.h \
    $${BOI_INCLUDE_DIR}/Action.h \
    $${BOI_INCLUDE_DIR}/ActionArgs.h \
    $${BOI_INCLUDE_DIR}/ActionCommand.h \
    $${BOI_INCLUDE_DIR}/BOILib.h \
    $${BOI_INCLUDE_DIR}/Component.h \
    $${BOI_INCLUDE_DIR}/CRef.h \
    $${BOI_INCLUDE_DIR}/CRefList.h \
    $${BOI_INCLUDE_DIR}/CSI.h \
    $${BOI_INCLUDE_DIR}/Data.h \
    $${BOI_INCLUDE_DIR}/Description.h \
    $${BOI_INCLUDE_DIR}/DescriptionRegistrar.h \
    $${BOI_INCLUDE_DIR}/DRef.h \
    $${BOI_INCLUDE_DIR}/Events/KeyEvent.h \
    $${BOI_INCLUDE_DIR}/Events/TouchEvent.h \
    $${BOI_INCLUDE_DIR}/Factory.h \
    $${BOI_INCLUDE_DIR}/FactoryRegistrar.h \
    $${BOI_INCLUDE_DIR}/InputMode.h \
    $${BOI_INCLUDE_DIR}/Instance.h \
    $${BOI_INCLUDE_DIR}/Interface.h \
    $${BOI_INCLUDE_DIR}/MenuItem.h \
    $${BOI_INCLUDE_DIR}/MenuSet.h \
    $${BOI_INCLUDE_DIR}/MenuRegistrar.h \
    $${BOI_INCLUDE_DIR}/Mutex.h \
    $${BOI_INCLUDE_DIR}/Object.h \
    $${BOI_INCLUDE_DIR}/Plugin.h \
    $${BOI_INCLUDE_DIR}/ViewLayerId.h \
    $${BOI_INCLUDE_DIR}/StandardMenus.h \
    $${BOI_INCLUDE_DIR}/StandardActions.h \
    $${BOI_INCLUDE_DIR}/StandardDataTypes.h \
    $${BOI_INCLUDE_DIR}/StandardReceivers.h \
    $${BOI_INCLUDE_DIR}/StandardEmitters.h \
    $${BOI_INCLUDE_DIR}/StandardComponents.h \
    $${BOI_INCLUDE_DIR}/StateId.h \
    $${BOI_INCLUDE_DIR}/TypeRegistrar.h \
    $${BOI_INCLUDE_DIR}/ViewTransformerId.h \
    $${BOI_INCLUDE_DIR}/Utilities/BoxInputHandler.h \
    $${BOI_INCLUDE_DIR}/Utilities/TouchVelocity.h \
    $${BOI_INCLUDE_DIR}/Utilities/Tuple.h \
    $${BOI_LIB_SRC_DIR}/Base.h \
    $${BOI_LIB_SRC_DIR}/View.h \
    $${BOI_LIB_SRC_DIR}/State.h \
    $${BOI_LIB_SRC_DIR}/Factories.h \
    $${BOI_LIB_SRC_DIR}/TypeManager.h \
    $${BOI_LIB_SRC_DIR}/ActionEngine.h \
    $${BOI_LIB_SRC_DIR}/ActionManager.h \
    $${BOI_LIB_SRC_DIR}/ActionChangedListener.h \
    $${BOI_LIB_SRC_DIR}/InputEventHandler.h \
    $${BOI_LIB_SRC_DIR}/DataManager.h \
    $${BOI_LIB_SRC_DIR}/GraphicsItem.h \
    $${BOI_LIB_SRC_DIR}/Connections.h \
    $${BOI_LIB_SRC_DIR}/ComponentData.h \
    $${BOI_LIB_SRC_DIR}/ComponentManager.h \
    $${BOI_LIB_SRC_DIR}/CRefListData.h \
    $${BOI_LIB_SRC_DIR}/CRefListPool.h \
    $${BOI_LIB_SRC_DIR}/ISI.h \
    $${BOI_LIB_SRC_DIR}/GuiRequest.h \
    $${BOI_LIB_SRC_DIR}/GuiRequestPool.h \
    $${BOI_LIB_SRC_DIR}/GuiRequestHandler.h \
    $${BOI_LIB_SRC_DIR}/Task.h \
    $${BOI_LIB_SRC_DIR}/TaskPool.h \
    $${BOI_LIB_SRC_DIR}/TaskProcessor.h \
    $${BOI_LIB_SRC_DIR}/MutexBase.h \
    $${BOI_LIB_SRC_DIR}/ThreadLockData.h \
    $${BOI_LIB_SRC_DIR}/MenuManager.h \
    $${BOI_LIB_SRC_DIR}/TagManager.h \
    $${BOI_LIB_SRC_DIR}/PluginManager.h \
    $${BOI_LIB_SRC_DIR}/ViewTransformer.h \
    $${BOI_LIB_SRC_DIR}/TransformManager.h \
    $${BOI_LIB_SRC_DIR}/CollectionManager.h \
    $${BOI_LIB_SRC_DIR}/DescriptionManager.h \
    $${BOI_LIB_SRC_DIR}/Events/CloseEventListener.h \
    $${BOI_LIB_SRC_DIR}/Events/EventDispatcher.h \
    $${BOI_LIB_SRC_DIR}/Events/KeyEventListener.h \
    $${BOI_LIB_SRC_DIR}/Events/TouchEventListener.h \
    $${BOI_LIB_SRC_DIR}/Events/ResizeEvent.h \
    $${BOI_LIB_SRC_DIR}/Events/ResizeEventListener.h \
    $${BOI_LIB_SRC_DIR}/Events/GuiRequestEvent.h \
    $${BOI_LIB_SRC_DIR}/Events/GuiRequestEventListener.h \
    $${BOI_LIB_SRC_DIR}/Events/SetActionEvent.h \
    $${BOI_LIB_SRC_DIR}/Events/SetActionEventListener.h \
    $${BOI_LIB_SRC_DIR}/Events/UpdateActionEvent.h \
    $${BOI_LIB_SRC_DIR}/Events/UpdateActionEventListener.h \
    $${BOI_LIB_SRC_DIR}/Events/InputModeChangeEvent.h \
    $${BOI_LIB_SRC_DIR}/Events/InputModeChangeEventListener.h \
    $${BOI_LIB_SRC_DIR}/Actions/XGestureAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/HiderAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/PanAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ZoomAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/NullAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/MoveAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/CopyAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/PasteAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/InsertAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/DeleteAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ResizeAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ConnectAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/CollectAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/LoadUrlAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/PutTextAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ShutdownAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ResetViewAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/SetChildAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/SetNumberAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/RectSelectAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ZoomToRectAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/InsertTextAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/AppendTextAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/FreeRotateAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/SetRotationAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/MoveToLayerAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/AddToLayoutAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/CaptureViewAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/FitAllInViewAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/InsertBrowserAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ToggleHotSpotAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ResetViewScaleAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ClearSelectionAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/InsertCollectionAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/RemoveFromLayoutAction.h \
    $${BOI_LIB_SRC_DIR}/Actions/ToggleFullScreenAction.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/IntData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/CharData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/BoolData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/ShortData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/FloatData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/DoubleData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/StringData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/ImageData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/SizeData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/RectData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/ColorData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/BrushData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/FontData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/PenData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/PointData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/MenuSetData.h \
    $${BOI_LIB_SRC_DIR}/DataTypes/BoundingBoxData.h \
    $${BOI_LIB_SRC_DIR}/Factories/CoreActionsFactory.h \
    $${BOI_LIB_SRC_DIR}/Factories/CustomEventsFactory.h \
    $${BOI_LIB_SRC_DIR}/Factories/StandardComponentsFactory.h \
    $${BOI_LIB_SRC_DIR}/Factories/PrimitiveTypesDataFactory.h \
    $${BOI_LIB_SRC_DIR}/Factories/CoreDataTypesFactory.h \
    $${BOI_LIB_SRC_DIR}/Factories/TimerComponentFactory.h \
    $${BOI_LIB_SRC_DIR}/Components/RootMenu.h \
    $${BOI_LIB_SRC_DIR}/Components/ContextMenu.h \
    $${BOI_LIB_SRC_DIR}/Components/StandardMenu.h \
    $${BOI_LIB_SRC_DIR}/Components/MenuComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/TextComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/TimerComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/ImageComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/BrowserComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/XGestureComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/TextInputComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/WindowSizeComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/JavascriptComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/BasicComponentDrawData.h \
    $${BOI_LIB_SRC_DIR}/Components/VerticalLayoutComponent.h \
    $${BOI_LIB_SRC_DIR}/Components/HorizontalLayoutComponent.h \
    $${BOI_LIB_SRC_DIR}/ViewTransformers/ScrollViewTransformer.h \
    $${BOI_LIB_SRC_DIR}/ViewTransformers/KineticScrollViewTransformer.h \

SOURCES += \
    $${BOI_LIB_SRC_DIR}/Instance.cpp \
    $${BOI_LIB_SRC_DIR}/Base.cpp \
    $${BOI_LIB_SRC_DIR}/View.cpp \
    $${BOI_LIB_SRC_DIR}/State.cpp \
    $${BOI_LIB_SRC_DIR}/Factories.cpp \
    $${BOI_LIB_SRC_DIR}/TypeManager.cpp \
    $${BOI_LIB_SRC_DIR}/Action.cpp \
    $${BOI_LIB_SRC_DIR}/ActionEngine.cpp \
    $${BOI_LIB_SRC_DIR}/ActionManager.cpp \
    $${BOI_LIB_SRC_DIR}/InputEventHandler.cpp \
    $${BOI_LIB_SRC_DIR}/DRef.cpp \
    $${BOI_LIB_SRC_DIR}/DataManager.cpp \
    $${BOI_LIB_SRC_DIR}/GraphicsItem.cpp \
    $${BOI_LIB_SRC_DIR}/Interface.cpp \
    $${BOI_LIB_SRC_DIR}/Connections.cpp \
    $${BOI_LIB_SRC_DIR}/Component.cpp \
    $${BOI_LIB_SRC_DIR}/ComponentData.cpp \
    $${BOI_LIB_SRC_DIR}/ComponentManager.cpp \
    $${BOI_LIB_SRC_DIR}/CRef.cpp \
    $${BOI_LIB_SRC_DIR}/CRefList.cpp \
    $${BOI_LIB_SRC_DIR}/CRefListPool.cpp \
    $${BOI_LIB_SRC_DIR}/ASI.cpp \
    $${BOI_LIB_SRC_DIR}/CSI.cpp \
    $${BOI_LIB_SRC_DIR}/ISI.cpp \
    $${BOI_LIB_SRC_DIR}/GuiRequest.cpp \
    $${BOI_LIB_SRC_DIR}/GuiRequestPool.cpp \
    $${BOI_LIB_SRC_DIR}/GuiRequestHandler.cpp \
    $${BOI_LIB_SRC_DIR}/TaskPool.cpp \
    $${BOI_LIB_SRC_DIR}/TaskProcessor.cpp \
    $${BOI_LIB_SRC_DIR}/Mutex.cpp \
    $${BOI_LIB_SRC_DIR}/MutexBase.cpp \
    $${BOI_LIB_SRC_DIR}/MenuItem.cpp \
    $${BOI_LIB_SRC_DIR}/MenuManager.cpp \
    $${BOI_LIB_SRC_DIR}/TagManager.cpp \
    $${BOI_LIB_SRC_DIR}/PluginManager.cpp \
    $${BOI_LIB_SRC_DIR}/StandardMenus.cpp \
    $${BOI_LIB_SRC_DIR}/StandardActions.cpp \
    $${BOI_LIB_SRC_DIR}/StandardDataTypes.cpp \
    $${BOI_LIB_SRC_DIR}/StandardReceivers.cpp \
    $${BOI_LIB_SRC_DIR}/StandardEmitters.cpp \
    $${BOI_LIB_SRC_DIR}/StandardComponents.cpp \
    $${BOI_LIB_SRC_DIR}/ViewTransformer.cpp \
    $${BOI_LIB_SRC_DIR}/TransformManager.cpp \
    $${BOI_LIB_SRC_DIR}/CollectionManager.cpp \
    $${BOI_LIB_SRC_DIR}/DescriptionManager.cpp \
    $${BOI_LIB_SRC_DIR}/Events/EventDispatcher.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/XGestureAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/HiderAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/PanAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ZoomAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/NullAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/MoveAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/CopyAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/PasteAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/InsertAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/DeleteAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ResizeAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ConnectAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/CollectAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/LoadUrlAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/PutTextAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ShutdownAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ResetViewAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/SetChildAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/SetNumberAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/RectSelectAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ZoomToRectAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/InsertTextAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/AppendTextAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/FreeRotateAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/SetRotationAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/MoveToLayerAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/AddToLayoutAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/CaptureViewAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/FitAllInViewAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/InsertBrowserAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ToggleHotSpotAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ResetViewScaleAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ClearSelectionAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/InsertCollectionAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/RemoveFromLayoutAction.cpp \
    $${BOI_LIB_SRC_DIR}/Actions/ToggleFullScreenAction.cpp \
    $${BOI_LIB_SRC_DIR}/Factories/CoreActionsFactory.cpp \
    $${BOI_LIB_SRC_DIR}/Factories/CustomEventsFactory.cpp \
    $${BOI_LIB_SRC_DIR}/Factories/StandardComponentsFactory.cpp \
    $${BOI_LIB_SRC_DIR}/Factories/PrimitiveTypesDataFactory.cpp \
    $${BOI_LIB_SRC_DIR}/Factories/CoreDataTypesFactory.cpp \
    $${BOI_LIB_SRC_DIR}/Factories/TimerComponentFactory.cpp \
    $${BOI_LIB_SRC_DIR}/Utilities/BoxInputHandler.cpp \
    $${BOI_LIB_SRC_DIR}/Utilities/TouchVelocity.cpp \
    $${BOI_LIB_SRC_DIR}/Components/RootMenu.cpp \
    $${BOI_LIB_SRC_DIR}/Components/ContextMenu.cpp \
    $${BOI_LIB_SRC_DIR}/Components/StandardMenu.cpp \
    $${BOI_LIB_SRC_DIR}/Components/MenuComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/TextComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/TimerComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/ImageComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/BrowserComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/XGestureComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/TextInputComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/WindowSizeComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/JavascriptComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/BasicComponentDrawData.cpp \
    $${BOI_LIB_SRC_DIR}/Components/VerticalLayoutComponent.cpp \
    $${BOI_LIB_SRC_DIR}/Components/HorizontalLayoutComponent.cpp \
    $${BOI_LIB_SRC_DIR}/ViewTransformers/ScrollViewTransformer.cpp \
    $${BOI_LIB_SRC_DIR}/ViewTransformers/KineticScrollViewTransformer.cpp \

