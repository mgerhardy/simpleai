LIBS += -lsimpleai
INCLUDEPATH += /usr/include/simpleai

TEMPLATE = app
TARGET = simpleai-debugger
DEPENDPATH += . \
              src \
              src/gui
INCLUDEPATH += . src src/gui src/gui/dialog src/gui/widget src/gui/view/map src/gui/view/tree src/gui/view/aggro src/gui/view/state src/gui/view/list src/common

CONFIG += qt debug_and_release

QMAKE_CXXFLAGS += -std=c++0x

# Input
HEADERS += \
           src/AIApplication.h \
           src/AIDebugger.h \
           src/common/Settings.h \
           src/common/Common.h \
           src/gui/AIDebuggerWidget.h \
           src/gui/dialog/ConnectDialog.h \
           src/gui/dialog/IDialog.h \
           src/gui/view/list/EntityList.h \
           src/gui/view/list/EntityListModel.h \
           src/gui/view/map/MapView.h \
           src/gui/view/map/MapItem.h \
           src/gui/view/tree/NodeTreeItem.h \
           src/gui/view/tree/NodeTreeView.h \
           src/gui/view/state/StateTable.h \
           src/gui/view/state/StateTableModel.h \
           src/gui/view/aggro/AggroTable.h \
           src/gui/view/aggro/AggroTableModel.h \
           src/gui/widget/ZoomFrame.h \
           src/gui/widget/ZoomWidget.h \
           src/gui/widget/IComboBox.h \
           src/gui/widget/IGraphicsView.h

SOURCES += \
           src/Main.cpp \
           src/AIApplication.cpp \
           src/AIDebugger.cpp \
           src/gui/AIDebuggerWidget.cpp \
           src/gui/dialog/ConnectDialog.cpp \
           src/gui/dialog/IDialog.cpp \
           src/gui/view/list/EntityList.cpp \
           src/gui/view/list/EntityListModel.cpp \
           src/gui/view/map/MapView.cpp \
           src/gui/view/map/MapItem.cpp \
           src/gui/view/tree/NodeTreeItem.cpp \
           src/gui/view/tree/NodeTreeView.cpp \
           src/gui/view/state/StateTable.cpp \
           src/gui/view/state/StateTableModel.cpp \
           src/gui/view/aggro/AggroTable.cpp \
           src/gui/view/aggro/AggroTableModel.cpp \
           src/gui/widget/ZoomFrame.cpp \
           src/gui/widget/ZoomWidget.cpp \
           src/gui/widget/IComboBox.cpp \
           src/gui/widget/IGraphicsView.cpp

RESOURCES += data/resources.qrc
FORMS +=
TRANSLATIONS += data/simpleai_de_DE.ts

win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
unix:QMAKE_LRELEASE = lrelease

QMAKE_EXTRA_COMPILERS += lrelease
lrelease.input = TRANSLATIONS
lrelease.output = data/${QMAKE_FILE_BASE}.qm
lrelease.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -o data/${QMAKE_FILE_BASE}.qm
lrelease.CONFIG = no_link target_predeps
#PRE_TARGETDEPS += compiler_lrelease_make_all

QMAKE_RESOURCE_FLAGS += -threshold 0 -compress 9

win32:RC_FILE = src/app.rc
#unix:ICON = src/images/icon.icns

MOC_DIR = .moc
UI_DIR = .uic
RCC_DIR = .rcc
OBJECTS_DIR = .obj
QT += gui widgets network
