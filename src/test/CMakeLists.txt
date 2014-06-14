if (SIMPLEAI_TEST)
	include_directories(${SIMPLEAI_SOURCE_DIR}/src/ai ${SIMPLEAI_SOURCE_DIR}/src/test/gtest ${SIMPLEAI_SOURCE_DIR}/src/test/gtest/include)
	link_directories(${SIMPLEAI_BINARY_DIR}/src/ai)
	file(GLOB_RECURSE SRC ${SIMPLEAI_SOURCE_DIR}/src/test/*.cc ${SIMPLEAI_SOURCE_DIR}/src/test/*.cpp ${SIMPLEAI_SOURCE_DIR}/src/test/*.h)
	add_executable(tests ${SRC})
	target_link_libraries(tests simpleai)

	if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
	elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
		target_link_libraries(tests pthread)
	endif()
endif()

