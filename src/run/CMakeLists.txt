set(SRC
	GameEntity.cpp GameEntity.h
	GameMap.h
	Main.cpp
)

set(SIMPLEAI_RUN_DEFINITIONS)
if (SIMPLEAI_LUA)
	list(APPEND SIMPLEAI_RUN_DEFINITIONS AI_INCLUDE_LUA)
endif()

if (SIMPLEAI_XML)
	list(APPEND SIMPLEAI_RUN_DEFINITIONS AI_INCLUDE_XML)
endif()

include_directories(${SIMPLEAI_SOURCE_DIR}/src/ai)
link_directories(${SIMPLEAI_BINARY_DIR}/src/ai)

add_executable(simpleai-run ${SRC})
target_link_libraries(simpleai-run simpleai)
if (SIMPLEAI_RUN_DEFINITIONS)
	target_compile_definitions(simpleai-run PRIVATE ${SIMPLEAI_RUN_DEFINITIONS})
endif()

check_lua_files(simpleai-run behaviours.lua)