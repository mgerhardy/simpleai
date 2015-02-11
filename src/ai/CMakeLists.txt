file(GLOB_RECURSE SRC *.cpp *.h)

set(DEPS ${CMAKE_THREAD_LIBS_INIT} ${GOOGLE_PROFILER_LIB} ${GOOGLE_TCMALLOC_LIB})

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	if (CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wabi -Wcast-qual -Wconversion -Wfloat-equal -Wmissing-declarations -Wmissing-format-attribute -Wno-long-long -Woverloaded-virtual -Wpacked -Wredundant-decls -Wshadow -Wsign-promo -Wno-sign-compare")
	endif()
endif()

if (SIMPLEAI_LUA)
	message(STATUS "Build lib with lua support")
	find_package(Lua)
	if (NOT LUA_FOUND)
		message(STATUS "Use embedded lua")
		include_directories(${SIMPLEAI_SOURCE_DIR}/src/libs/lua/)
		list(APPEND DEPS lua)
	endif()
else()
	message(STATUS "Build lib without lua support")
	file(GLOB LUA_REMOVE tree/loaders/lua/*)
	list(REMOVE_ITEM SRC ${LUA_REMOVE})
endif()

if (SIMPLEAI_XML)
	message(STATUS "Build lib with XML support")
	include_directories(${SIMPLEAI_SOURCE_DIR}/src/libs/tinyxml2/)
	list(APPEND DEPS tinyxml2)
else()
	message(STATUS "Build lib without XML support")
	file(GLOB XML_REMOVE tree/loaders/xml/*)
	list(REMOVE_ITEM SRC ${XML_REMOVE})
endif()

find_package(Threads)

include_directories(. ${LUA_INCLUDE_DIR})
add_library(simpleai ${SRC})
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	list(APPEND DEPS -latomic)
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
	server/AIChangeMessage.h
	server/AICharacterDetailsMessage.h
	server/AICharacterStaticMessage.h
	server/AINamesMessage.h
	server/AIPauseMessage.h
	server/AISelectMessage.h
	server/AIStateMessage.h
	server/AIStepMessage.h
	server/AIStubTypes.h
	server/ChangeHandler.h
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
	zone/Zone.h
	SimpleAI.h
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
