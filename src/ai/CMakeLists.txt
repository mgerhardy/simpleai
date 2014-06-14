file(GLOB_RECURSE SRC *.cpp *.h)

if (SIMPLEAI_LUA)
	include_directories(${SIMPLEAI_SOURCE_DIR}/src/libs/lua/)
	file(GLOB_RECURSE SRC_LUA ${SIMPLEAI_SOURCE_DIR}/src/libs/lua/*.cpp ${SIMPLEAI_SOURCE_DIR}/src/libs/lua/*.h)
	list(APPEND SRC ${SRC_LUA})
else()
	list(REMOVE_ITEM SRC tree/loaders/lua/LUA.cpp tree/loaders/lua/LUANode.cpp tree/loaders/lua/LUATree.cpp tree/loaders/lua/LUATreeLoader.cpp)
endif()

include_directories(.)
add_library(simpleai ${SRC})

# TODO Write Config.h with version and Stuff
#VERSION=0.1
#src/ai/Config.h
