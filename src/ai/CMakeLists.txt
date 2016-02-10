include(CheckFunctionExists)
include(CheckLibraryExists)

set(SRC
	group/GroupMgr.cpp
	AI.cpp
	server/AddNodeHandler.cpp
	server/UpdateNodeHandler.cpp
	server/DeleteNodeHandler.cpp
	server/ResetHandler.cpp
	server/StepHandler.cpp
	server/SelectHandler.cpp
	server/ProtocolMessageFactory.cpp
	server/Server.cpp
	server/PauseHandler.cpp
	server/Network.cpp
	server/ChangeHandler.cpp
	filter/SelectZone.cpp
	filter/SelectHighestAggro.cpp
	filter/SelectEmpty.cpp
	filter/SelectGroupMembers.cpp
	filter/SelectGroupLeader.cpp
	tree/Fail.cpp
	tree/Limit.cpp
	tree/Sequence.cpp
	tree/Invert.cpp
	tree/Succeed.cpp
	tree/TreeNode.cpp
	tree/Parallel.cpp
	tree/TreeNodeParser.cpp
	tree/Print.cpp
	tree/Steer.cpp
	tree/Idle.cpp
	tree/RandomSelector.cpp
	tree/PrioritySelector.cpp
	tree/ITask.cpp
	tree/ProbabilitySelector.cpp
	tree/ITimedNode.cpp
	conditions/And.cpp
	conditions/ConditionParser.cpp
	conditions/Not.cpp
	conditions/Or.cpp
	conditions/True.cpp
	conditions/IsInGroup.cpp
	conditions/HasEnemies.cpp
	conditions/IsCloseToGroup.cpp
	conditions/IsGroupLeader.cpp
	conditions/ICondition.cpp
	conditions/Filter.cpp
	conditions/False.cpp
	common/Vector3f.cpp
	AIRegistry.cpp
	zone/Zone.cpp
	movement/Steering.cpp
	aggro/AggroMgr.cpp
)

if (SIMPLEAI_LUA)
	message(STATUS "Build lib with lua support")
	find_package(Lua)
	if (NOT LUA_FOUND)
		message(STATUS "Use embedded lua")
		include_directories(${SIMPLEAI_SOURCE_DIR}/src/libs/lua/)
		list(APPEND DEPS lua)
	else()
		include_directories(${LUA_INCLUDE_DIR})
	endif()
	list(APPEND SRC
		tree/loaders/lua/LUANode.cpp
		tree/loaders/lua/LUA.cpp
		tree/loaders/lua/LUATreeLoader.cpp
		tree/loaders/lua/LUATree.cpp
	)
else()
	message(STATUS "Build lib without lua support")
	file(GLOB LUA_REMOVE tree/loaders/lua/*)
	list(REMOVE_ITEM SRC ${LUA_REMOVE})
endif()

if (SIMPLEAI_XML)
	message(STATUS "Build lib with XML support")
	include_directories(${SIMPLEAI_SOURCE_DIR}/src/libs/tinyxml2/)
	list(APPEND DEPS tinyxml2)
	list(APPEND SRC tree/loaders/xml/XMLTreeLoader.cpp)
else()
	message(STATUS "Build lib without XML support")
	file(GLOB XML_REMOVE tree/loaders/xml/*)
	list(REMOVE_ITEM SRC ${XML_REMOVE})
endif()

find_package(Threads)

include_directories(.)
add_library(simpleai ${SRC} ${simpleai_HEADER_DIRS})
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	check_function_exists(__atomic_fetch_add_4 HAVE___ATOMIC_FETCH_ADD_4)
	if (NOT HAVE___ATOMIC_FETCH_ADD_4)
		check_library_exists(atomic __atomic_fetch_add_4 "" HAVE_LIBATOMIC)
		if (HAVE_LIBATOMIC)
			list(APPEND DEPS "-latomic")
		endif()
	endif()
endif()
target_link_libraries(simpleai ${DEPS} ${LUA_LIBRARIES})

list(APPEND simpleai_HEADERS
	aggro/AggroMgr.h
	aggro/Entry.h
	AI.h
	AIRegistry.h
	common/Vector3f.h
	common/IFactoryRegistry.h
	common/IParser.h
	common/IPrintable.h
	common/Math.h
	common/MemoryAllocator.h
	common/MoveVector.h
	common/NonCopyable.h
	common/Random.h
	common/String.h
	common/Thread.h
	common/ThreadPool.h
	common/ThreadScheduler.h
	common/Types.h
	conditions/And.h
	conditions/ConditionParser.h
	conditions/False.h
	conditions/Filter.h
	conditions/HasEnemies.h
	conditions/ICondition.h
	conditions/IsCloseToGroup.h
	conditions/IsGroupLeader.h
	conditions/IsInGroup.h
	conditions/Not.h
	conditions/Or.h
	conditions/True.h
	filter/IFilter.h
	filter/SelectEmpty.h
	filter/SelectGroupLeader.h
	filter/SelectGroupMembers.h
	filter/SelectHighestAggro.h
	filter/SelectZone.h
	IAIFactory.h
	ICharacter.h
	group/GroupMgr.h
	movement/SelectionSeek.h
	movement/GroupFlee.h
	movement/GroupSeek.h
	movement/Steering.h
	movement/TargetFlee.h
	movement/TargetSeek.h
	movement/Wander.h
	movement/WeightedSteering.h
	server/AIAddNodeMessage.h
	server/AIChangeMessage.h
	server/AICharacterDetailsMessage.h
	server/AICharacterStaticMessage.h
	server/AIDeleteNodeMessage.h
	server/AINamesMessage.h
	server/AIPauseMessage.h
	server/AISelectMessage.h
	server/AIStateMessage.h
	server/AIStepMessage.h
	server/AIStubTypes.h
	server/AIUpdateNodeMessage.h
	server/AddNodeHandler.h
	server/ChangeHandler.h
	server/DeleteNodeHandler.h
	server/IProtocolHandler.h
	server/IProtocolMessage.h
	server/Network.h
	server/PauseHandler.h
	server/ProtocolHandlerRegistry.h
	server/ProtocolMessageFactory.h
	server/ResetHandler.h
	server/SelectHandler.h
	server/Server.h
	server/StepHandler.h
	server/UpdateNodeHandler.h
	zone/Zone.h
	SimpleAI.h
	tree/Fail.h
	tree/Limit.h
	tree/Idle.h
	tree/Invert.h
	tree/ITask.h
	tree/ITimedNode.h
	tree/loaders/ITreeLoader.h
	tree/Parallel.h
	tree/Print.h
	tree/PrioritySelector.h
	tree/ProbabilitySelector.h
	tree/RandomSelector.h
	tree/Selector.h
	tree/Sequence.h
	tree/Steer.h
	tree/Succeed.h
	tree/TreeNode.h
	tree/TreeNodeParser.h
)

if (SIMPLEAI_LUA)
	list(APPEND simpleai_HEADERS
		tree/loaders/lua/LUACondition.h
		tree/loaders/lua/LUAFunctions.h
		tree/loaders/lua/LUA.h
		tree/loaders/lua/LUANode.h
		tree/loaders/lua/LUATree.h
		tree/loaders/lua/LUATreeLoader.h
	)
endif()

if (SIMPLEAI_XML)
	list(APPEND simpleai_HEADERS
		tree/loaders/xml/XMLTreeLoader.h
	)
endif()

set_target_properties(simpleai PROPERTIES VERSION ${SIMPLEAI_VERSION} SOVERSION ${SIMPLEAI_VERSION})

macro(INSTALL_HEADERS_WITH_DIRECTORY HEADER_LIST)
foreach(HEADER ${${HEADER_LIST}})
STRING(REGEX MATCH "(.*)[/\\]" DIR ${HEADER})
install(FILES ${HEADER} DESTINATION include/simpleai/${DIR})
endforeach(HEADER)
endmacro(INSTALL_HEADERS_WITH_DIRECTORY)

include(FindPkgConfig QUIET)
if (PKG_CONFIG_FOUND)
	set(prefix ${CMAKE_INSTALL_PREFIX})
	set(exec_prefix "\${prefix}")
	set(libdir "\${exec_prefix}/lib${LIB_SUFFIX}")
	set(bindir "\${exec_prefix}/bin")
	set(includedir "\${prefix}/include")
	configure_file(${SIMPLEAI_SOURCE_DIR}/libsimpleai.pc.in ${SIMPLEAI_BINARY_DIR}/libsimpleai.pc @ONLY)
	install(FILES ${SIMPLEAI_BINARY_DIR}/libsimpleai.pc DESTINATION "lib${LIB_SUFFIX}/pkgconfig")
endif()

#install(FILES ${SIMPLEAI_SOURCE_DIR}/FindSimpleAI.cmake DESTINATION ${CMAKE_MODULE_PATH})

install(TARGETS simpleai ARCHIVE DESTINATION lib)
install(DIRECTORY ${simpleai_HEADER_DIRS} DESTINATION include/simpleai)
#install(EXPORT simpleai DESTINATION lib FILE libsimpleai.cmake)
INSTALL_HEADERS_WITH_DIRECTORY(simpleai_HEADERS)

# TODO Write Config.h with version and Stuff
#VERSION=0.1
#src/ai/Config.h
