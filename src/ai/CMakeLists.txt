file(GLOB_RECURSE SRC *.cpp *.h)

if (SIMPLEAI_LUA)
	message(STATUS "Build with lua support")
	include_directories(${SIMPLEAI_SOURCE_DIR}/src/libs/lua/)
	file(GLOB_RECURSE SRC_LUA ${SIMPLEAI_SOURCE_DIR}/src/libs/lua/*.cpp ${SIMPLEAI_SOURCE_DIR}/src/libs/lua/*.h)
	list(APPEND SRC ${SRC_LUA})
else()
	message(STATUS "Build without lua support")
	# TODO doesn't work - still compile LUA.cpp and friends if run with -DSIMPLEAI_LUA=FALSE
	list(REMOVE_ITEM SRC
		tree/loaders/lua/LUA.cpp
		tree/loaders/lua/LUA.h
		tree/loaders/lua/LUANode.cpp
		tree/loaders/lua/LUANode.h
		tree/loaders/lua/LUATree.cpp
		tree/loaders/lua/LUATree.h
		tree/loaders/lua/LUACondition.h
		tree/loaders/lua/LUAFunctions.h
		tree/loaders/lua/LUATreeLoader.cpp
		tree/loaders/lua/LUATreeLoader.h)
endif()

find_package(Threads)

include_directories(.)
add_library(simpleai ${SRC})
target_link_libraries(simpleai ${CMAKE_THREAD_LIBS_INIT} -latomic)

list(APPEND simpleai_HEADERS
	aggro/AggroMgr.h
	aggro/Entry.h
	AI.h
	AIRegistry.h
	common/AIException.h
	common/Vector3f.h
	common/Enum.h
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
	filter/SelectHighestAggro.h
	IAIFactory.h
	ICharacter.h
	group/GroupMgr.h
	movement/Steering.h
	server/AIChangeMessage.h
	server/AICharacterDetailsMessage.h
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
	server/Zone.h
	SimpleAI.h
	tree/group/FleeGroup.h
	tree/group/FollowGroup.h
	tree/Idle.h
	tree/Invert.h
	tree/ITask.h
	tree/ITimedNode.h
	tree/loaders/ITreeLoader.h
	tree/Parallel.h
	tree/Print.h
	tree/PrioritySelector.h
	tree/Selector.h
	tree/Sequence.h
	tree/TreeNode.h
	tree/TreeNodeParser.h
	tree/Wander.h
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
