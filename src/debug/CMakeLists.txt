cmake_minimum_required(VERSION 2.8.8)
project(simpleai-debugger)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(libsimpleai REQUIRED)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Gui REQUIRED)

list(APPEND simpleai_debugger_SOURCES
           src/Main.cpp
           src/AIApplication.cpp
           src/AIDebugger.cpp
           src/gui/AIDebuggerWidget.cpp
           src/gui/dialog/ConnectDialog.cpp
           src/gui/dialog/IDialog.cpp
           src/gui/view/list/EntityList.cpp
           src/gui/view/list/EntityListModel.cpp
           src/gui/view/map/MapView.cpp
           src/gui/view/map/MapItem.cpp
           src/gui/view/tree/NodeTreeItem.cpp
           src/gui/view/tree/NodeTreeView.cpp
           src/gui/view/state/StateTable.cpp
           src/gui/view/state/StateTableModel.cpp
           src/gui/view/aggro/AggroTable.cpp
           src/gui/view/aggro/AggroTableModel.cpp
           src/gui/widget/ZoomFrame.cpp
           src/gui/widget/ZoomWidget.cpp
           src/gui/widget/IComboBox.cpp
           src/gui/widget/IGraphicsView.cpp)

add_executable(simpleai-debugger ${simpleai_debugger_SOURCES})

qt5_use_modules(simpleai-debugger Widgets Gui Network)
