LIBS += -lsimpleai
INCLUDEPATH += /usr/include/simpleai

TEMPLATE = app
TARGET = simpleai-debugger
DEPENDPATH += . \
              src \
              src/gui
INCLUDEPATH += . src src/gui src/gui/dialog src/gui/widget src/gui/view/map src/gui/view/tree src/gui/view/state src/gui/view/list src/common

CONFIG += qt

QMAKE_CXXFLAGS += -std=c++0x

# Input
HEADERS += src/AIDebugger.h \
           src/gui/AIDebuggerWindow.h \
           src/gui/dialog/ConnectDialog.h \
           src/gui/dialog/IDialog.h \
           src/gui/view/list/EntityList.h \
           src/gui/view/map/MapScene.h \
           src/gui/view/map/MapView.h \
           src/gui/view/map/MapItem.h \
           src/gui/view/tree/NodeTreeItem.h \
           src/gui/view/tree/NodeTreeWidget.h \
           src/gui/view/state/StateTable.h \
           src/gui/widget/IComboBox.h

SOURCES += src/Main.cpp \
           src/AIDebugger.cpp \
           src/gui/AIDebuggerWindow.cpp \
           src/gui/dialog/ConnectDialog.cpp \
           src/gui/dialog/IDialog.cpp \
           src/gui/view/list/EntityList.cpp \
           src/gui/view/map/MapScene.cpp \
           src/gui/view/map/MapView.cpp \
           src/gui/view/map/MapItem.cpp \
           src/gui/view/tree/NodeTreeItem.cpp \
           src/gui/view/tree/NodeTreeWidget.cpp \
           src/gui/view/state/StateTable.cpp \
           src/gui/widget/IComboBox.cpp

RESOURCES += data/resources.qrc
FORMS +=
TRANSLATIONS +=

win32:RC_FILE = src/app.rc
#unix:ICON = src/images/icon.icns

MOC_DIR = .moc
UI_DIR = .uic
RCC_DIR = .rcc
OBJECTS_DIR = .obj
QT += gui widgets network
