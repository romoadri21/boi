######################################################################
# BOI Application QMake build file
######################################################################

BOI_BASE_DIR = ../..

BOI_BIN_DIR     = $${BOI_BASE_DIR}/bin
BOI_LIB_DIR     = $${BOI_BASE_DIR}/lib
BOI_INCLUDE_DIR = $${BOI_BASE_DIR}/include

BOI_LIB_SRC_DIR = $${BOI_BASE_DIR}/src/lib
BOI_APP_SRC_DIR = $${BOI_BASE_DIR}/src/app

TEMPLATE    = app
TARGET      = BOI
CONFIG     += debug
DESTDIR     = $${BOI_BIN_DIR}
OBJECTS_DIR = .
MOC_DIR     = .
RCC_DIR     = .

DEPENDPATH  += $${BOI_INCLUDE_DIR}
INCLUDEPATH += $${BOI_INCLUDE_DIR} $${BOI_LIB_SRC_DIR}

LIBS += -L$${BOI_LIB_DIR} -lBOI

unix:PRE_TARGETDEPS  += $${BOI_LIB_DIR}/libBOI.so
win32:PRE_TARGETDEPS += $${BOI_LIB_DIR}/BOI.lib

SOURCES += $${BOI_APP_SRC_DIR}/main.cpp \

