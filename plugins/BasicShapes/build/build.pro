
BOI_BASE_DIR    = ../../..
PLUGIN_BASE_DIR = ..

BOI_BIN_DIR     = $${BOI_BASE_DIR}/bin
BOI_LIB_DIR     = $${BOI_BASE_DIR}/lib
BOI_INCLUDE_DIR = $${BOI_BASE_DIR}/include

PLUGIN_SRC_DIR = $${PLUGIN_BASE_DIR}/src

TEMPLATE    = lib
CONFIG     += plugin debug
TARGET      = BasicShapes
INFO_FILE   = BasicShapes.info
DESTDIR     = $${BOI_BIN_DIR}/plugins
OBJECTS_DIR = .
MOC_DIR     = .
RCC_DIR     = .

LIBS += -L$${BOI_LIB_DIR} -lBOI

InfoTarget.target = $${DESTDIR}/$${INFO_FILE}
InfoTarget.depends = $${PLUGIN_BASE_DIR}/$${INFO_FILE}
unix:InfoTarget.commands = cp -f $$InfoTarget.depends $$InfoTarget.target
win32:InfoTarget.commands = copy FORCE $$InfoTarget.depends $$InfoTarget.target
win32:InfoTarget.commands = $$replace(InfoTarget.commands,/,\\)
win32:InfoTarget.commands = $$replace(InfoTarget.commands,FORCE,/Y)

QMAKE_EXTRA_TARGETS += InfoTarget

unix:PRE_TARGETDEPS  += $${BOI_LIB_DIR}/libBOI.so $$InfoTarget.target
win32:PRE_TARGETDEPS += $${BOI_LIB_DIR}/BOI.lib   $$InfoTarget.target

INCLUDEPATH += $${BOI_INCLUDE_DIR} $${PLUGIN_SRC_DIR}

HEADERS += \
    $${PLUGIN_SRC_DIR}/BasicShapesPlugin.h \
    $${PLUGIN_SRC_DIR}/ComponentsFactory.h \
    $${PLUGIN_SRC_DIR}/ActionsFactory.h \
    $${PLUGIN_SRC_DIR}/RectComponent.h \
    $${PLUGIN_SRC_DIR}/InsertShapeAction.h \

SOURCES += \
    $${PLUGIN_SRC_DIR}/BasicShapesPlugin.cpp \
    $${PLUGIN_SRC_DIR}/ComponentsFactory.cpp \
    $${PLUGIN_SRC_DIR}/ActionsFactory.cpp \
    $${PLUGIN_SRC_DIR}/RectComponent.cpp \
    $${PLUGIN_SRC_DIR}/InsertShapeAction.cpp \

