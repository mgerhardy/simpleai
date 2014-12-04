if (SIMPLEAI_EXAMPLE)
	include_directories(${SIMPLEAI_SOURCE_DIR}/src/ai)
	link_directories(${SIMPLEAI_BINARY_DIR}/src/ai)
	file(GLOB_RECURSE SRC ${SIMPLEAI_SOURCE_DIR}/src/example/*.cpp ${SIMPLEAI_SOURCE_DIR}/src/example/*.h)
	add_executable(simpleai-run ${SRC})
	target_link_libraries(simpleai-run simpleai)
endif()

