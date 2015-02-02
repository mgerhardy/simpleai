cmake_minimum_required(VERSION 2.8.12)
project(simpleai-debugger)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# Tell CMake to run rcc when necessary:
set(CMAKE_AUTORCC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../ai/")
set(AUTOGEN_TARGETS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/.moc")
set(AUTOMOC_TARGETS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/.moc")

if (${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
	add_definitions(-DCMAKE_CXX_COMPILER_ARG1=-std=c++11)
	add_definitions(-DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE)
	add_definitions(-DCMAKE_ECLIPSE_VERSION=4.2)
endif()

find_package(SimpleAI REQUIRED)

option(SIMPLEAI_PROFILER "Use Google cpu profiler" OFF)

if (SIMPLEAI_PROFILER)
	FIND_LIBRARY(GOOGLE_PROFILER_LIB profiler)
	FIND_LIBRARY(GOOGLE_TCMALLOC_LIB tcmalloc)
	if (GOOGLE_PROFILER_LIB AND GOOGLE_TCMALLOC_LIB)
		message(STATUS "Found Google profiler: ${GOOGLE_PROFILER_LIB} ${GOOGLE_TCMALLOC_LIB}")
		add_definitions(-DAI_PROFILER)
	else()
		message(STATUS "Can't find Google profiler")
	endif()
endif()

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	if (CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -msse3 -Wall -Wextra -DNDEBUG")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG -Wall -Wextra -Werror -pedantic -Wabi -Wcast-qual -Wconversion -Wfloat-equal -Wmissing-declarations -Wmissing-format-attribute -Wno-long-long -Woverloaded-virtual -Wpacked -Wredundant-decls -Wshadow -Wsign-promo -Wno-sign-compare")
	else()
		message(ERROR "unsupported compiler")
	endif()
endif()

file(GLOB simpleai_debugger_TRANSLATIONS data/*.ts)

list(APPEND simpleai_debugger_HEADERS
           src/gui/AICompressorProxy.h
)

list(APPEND simpleai_debugger_SOURCES
           src/Main.cpp
           src/AIApplication.cpp
           src/AIDebugger.cpp
           src/gui/AIDebuggerWidget.cpp
           src/gui/AINodeStaticResolver.cpp
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
           src/gui/widget/IGraphicsView.cpp)

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

qt5_add_translation(simpleai_debugger_QM ${simpleai_debugger_TRANSLATIONS})
qt5_add_resources(simpleai_debugger_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/data/resources.qrc)
add_executable(simpleai-debugger ${simpleai_debugger_SOURCES} ${simpleai_debugger_QM} ${simpleai_debugger_RESOURCES} ${simpleai_debugger_HEADERS})
qt5_use_modules(simpleai-debugger Widgets Gui Network)
target_link_libraries(simpleai-debugger ${SIMPLEAI_LIBRARIES} ${GOOGLE_PROFILER_LIB} ${GOOGLE_TCMALLOC_LIB})
