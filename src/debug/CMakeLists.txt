cmake_minimum_required(VERSION 2.8.8)
project(simpleai-debugger)

# TODO resources are not yet compiled in

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# Tell CMake to run rcc when necessary:
set(CMAKE_AUTORCC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../ai/")
set(AUTOGEN_TARGETS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/.moc")
set(AUTOMOC_TARGETS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/.moc")

find_package(SimpleAI REQUIRED)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Gui REQUIRED)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	if (CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -msse3 -Wall -Wextra -DNDEBUG")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG -Wall -Wextra -Werror")
	else()
		message(ERROR "unsupported compiler")
	endif()
endif()

list(APPEND simpleai_debugger_HEADERS
           src/gui/AICompressorProxy.h
)

list(APPEND simpleai_debugger_SOURCES
           data/resources.qrc
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

add_executable(simpleai-debugger ${simpleai_debugger_SOURCES} ${simpleai_debugger_HEADERS})
include_directories(
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${AUTOGEN_TARGETS_FOLDER}
           ${AUTOMOC_TARGETS_FOLDER}
           ${CMAKE_CURRENT_SOURCE_DIR}/src
           ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
           ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/dialog
           ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/widget
           ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/map
           ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/tree
           ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/aggro
           ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/state
           ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/view/list
           ${CMAKE_CURRENT_SOURCE_DIR}/src/common
           ${SIMPLEAI_INCLUDE_DIRS})

qt5_use_modules(simpleai-debugger Widgets Gui Network)
target_link_libraries(simpleai-debugger ${SIMPLEAI_LIBRARIES})
