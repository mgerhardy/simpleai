project(simpleai-debugger CXX)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# Tell CMake to run rcc when necessary:
set(CMAKE_AUTORCC ON)

set(AUTOGEN_TARGETS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/.moc")
set(AUTOMOC_TARGETS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/.moc")

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Gui REQUIRED)

set(simpleai_debugger_HEADERS
	src/gui/action/AddAction.h
	src/gui/action/DeleteAction.h
	src/gui/AICompressorProxy.h
	src/gui/dialog/AddDialog.h
	src/gui/dialog/IDialog.h
	src/gui/dialog/ConnectDialog.h
	src/gui/widget/ZoomFrame.h
	src/gui/widget/IGraphicsView.h
	src/gui/widget/IComboBox.h
	src/gui/widget/ZoomWidget.h
	src/gui/AINodeStaticResolver.h
	src/gui/view/map/MapItem.h
	src/gui/view/map/MapView.h
	src/gui/view/tree/TreeViewCommon.h
	src/gui/view/tree/BehaviourTreeModel.h
	src/gui/view/tree/NodeTreeView.h
	src/gui/view/tree/BehaviourTreeModelItem.h
	src/gui/view/tree/NodeTreeItem.h
	src/gui/view/list/EntityListModel.h
	src/gui/view/list/EntityList.h
	src/gui/view/aggro/AggroTable.h
	src/gui/view/aggro/AggroTableModel.h
	src/gui/view/state/StateTableModel.h
	src/gui/view/state/StateTable.h
	src/gui/AIDebuggerWidget.h
	src/AIApplication.h
	src/AIDebugger.h
	src/common/Settings.h
	src/common/Common.h
	src/Version.h
)

list(APPEND simpleai_debugger_SOURCES
	src/Main.cpp
	src/AIApplication.cpp
	src/AIDebugger.cpp
	src/gui/AIDebuggerWidget.cpp
	src/gui/AINodeStaticResolver.cpp
	src/gui/dialog/AddDialog.cpp
	src/gui/dialog/ConnectDialog.cpp
	src/gui/dialog/IDialog.cpp
	src/gui/view/list/EntityList.cpp
	src/gui/view/list/EntityListModel.cpp
	src/gui/view/map/MapView.cpp
	src/gui/view/map/MapItem.cpp
	src/gui/view/tree/BehaviourTreeModel.cpp
	src/gui/view/tree/BehaviourTreeModelItem.cpp
	src/gui/view/tree/NodeTreeItem.cpp
	src/gui/view/tree/NodeTreeView.cpp
	src/gui/view/tree/TreeViewCommon.cpp
	src/gui/view/state/StateTable.cpp
	src/gui/view/state/StateTableModel.cpp
	src/gui/view/aggro/AggroTable.cpp
	src/gui/view/aggro/AggroTableModel.cpp
	src/gui/widget/ZoomFrame.cpp
	src/gui/widget/ZoomWidget.cpp
	src/gui/widget/IComboBox.cpp
	src/gui/widget/IGraphicsView.cpp
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../ai
	${CMAKE_CURRENT_SOURCE_DIR}
	${AUTOGEN_TARGETS_FOLDER}
	${AUTOMOC_TARGETS_FOLDER}
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui/action
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui/dialog
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui/widget
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/map
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/tree
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/aggro
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/state
	${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/list
	${CMAKE_CURRENT_SOURCE_DIR}/src/common)

qt5_add_resources(simpleai_debugger_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/data/resources.qrc)
add_executable(simpleai-debugger ${simpleai_debugger_SOURCES} ${simpleai_debugger_RESOURCES} ${simpleai_debugger_HEADERS})
qt5_use_modules(simpleai-debugger Widgets Gui Network)
target_link_libraries(simpleai-debugger simpleai)
if (NOT MSVC)
	set_target_properties(simpleai-debugger PROPERTIES COMPILE_FLAGS "-Wno-inconsistent-missing-override")
endif()
