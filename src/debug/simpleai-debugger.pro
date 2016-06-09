LIBS += -lsimpleai
INCLUDEPATH += /usr/include/simpleai
INCLUDEPATH += /usr/local/include/simpleai
INCLUDEPATH += ../ai/simpleai

TEMPLATE = app
TARGET = simpleai-debugger
DEPENDPATH += . \
              src \
              src/gui
INCLUDEPATH += . src src/gui src/gui/dialog src/gui/widget src/gui/view/map src/gui/view/tree src/gui/view/aggro src/gui/view/state src/gui/action src/gui/view/list src/common

CONFIG += qt debug_and_release

QMAKE_CXXFLAGS += -std=c++11

# Input
HEADERS += \
           src/AIApplication.h \
           src/AIDebugger.h \
           src/common/Settings.h \
           src/common/Common.h \
           src/gui/AIDebuggerWidget.h \
           src/gui/AICompressorProxy.h \
           src/gui/AINodeStaticResolver.h \
           src/gui/dialog/AddDialog.h \
           src/gui/dialog/ConnectDialog.h \
           src/gui/dialog/IDialog.h \
           src/gui/dialog/SettingsDialog.h \
           src/gui/action/Action.h \
           src/gui/action/AddAction.h \
           src/gui/action/DeleteAction.h \
           src/gui/view/list/EntityList.h \
           src/gui/view/list/EntityListModel.h \
           src/gui/view/map/MapView.h \
           src/gui/view/map/MapItem.h \
           src/gui/view/tree/BehaviourTreeModel.h \
           src/gui/view/tree/BehaviourTreeModelItem.h \
           src/gui/view/tree/NodeTreeItem.h \
           src/gui/view/tree/NodeTreeView.h \
           src/gui/view/tree/TreeViewCommon.h \
           src/gui/view/state/StateTable.h \
           src/gui/view/state/StateTableModel.h \
           src/gui/view/aggro/AggroTable.h \
           src/gui/view/aggro/AggroTableModel.h \
           src/gui/widget/IComboBox.h

SOURCES += \
           src/Main.cpp \
           src/AIApplication.cpp \
           src/AIDebugger.cpp \
           src/gui/AIDebuggerWidget.cpp \
           src/gui/AINodeStaticResolver.cpp \
           src/gui/dialog/AddDialog.cpp \
           src/gui/dialog/ConnectDialog.cpp \
           src/gui/dialog/IDialog.cpp \
           src/gui/dialog/SettingsDialog.cpp \
           src/gui/view/list/EntityList.cpp \
           src/gui/view/list/EntityListModel.cpp \
           src/gui/view/map/MapView.cpp \
           src/gui/view/map/MapItem.cpp \
           src/gui/view/tree/BehaviourTreeModel.cpp \
           src/gui/view/tree/BehaviourTreeModelItem.cpp \
           src/gui/view/tree/NodeTreeItem.cpp \
           src/gui/view/tree/NodeTreeView.cpp \
           src/gui/view/tree/TreeViewCommon.cpp \
           src/gui/view/state/StateTable.cpp \
           src/gui/view/state/StateTableModel.cpp \
           src/gui/view/aggro/AggroTable.cpp \
           src/gui/view/aggro/AggroTableModel.cpp \
           src/gui/widget/IComboBox.cpp

RESOURCES += data/resources.qrc
FORMS +=
TRANSLATIONS +=

win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
unix:QMAKE_LRELEASE = lrelease

system($$QMAKE_LRELEASE data/simpleai_de_DE.ts data/simpleai_de_DE.qm)

QMAKE_RESOURCE_FLAGS += -threshold 0 -compress 9

win32:RC_FILE = src/app.rc
#unix:ICON = src/images/icon.icns

MOC_DIR = .moc
UI_DIR = .uic
RCC_DIR = .rcc
OBJECTS_DIR = .obj
QT += gui widgets network
