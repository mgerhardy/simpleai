find_package(Threads)

if (SIMPLEAI_RUN)
	include_directories(${SIMPLEAI_SOURCE_DIR}/src/ai)
	link_directories(${SIMPLEAI_BINARY_DIR}/src/ai)
	file(GLOB_RECURSE SRC ${SIMPLEAI_SOURCE_DIR}/src/run/*.cpp ${SIMPLEAI_SOURCE_DIR}/src/run/*.h)
	add_executable(simpleai-run ${SRC})
	target_link_libraries(simpleai-run simpleai ${CMAKE_THREAD_LIBS_INIT} -latomic)
endif()

