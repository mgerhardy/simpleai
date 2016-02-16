set(SRC
	GameEntity.cpp GameEntity.h
	GameMap.h
	Main.cpp
)

include_directories(${SIMPLEAI_SOURCE_DIR}/src/ai)
link_directories(${SIMPLEAI_BINARY_DIR}/src/ai)

add_executable(simpleai-run ${SRC})
target_link_libraries(simpleai-run simpleai)

check_lua_files(simpleai-run behaviours.lua)