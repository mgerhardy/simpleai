if (SIMPLEAI_TEST)
	find_package(Threads)

	include_directories(${SIMPLEAI_SOURCE_DIR}/src/ai ${SIMPLEAI_SOURCE_DIR}/src/test/gtest ${SIMPLEAI_SOURCE_DIR}/src/test/gtest/include)
	link_directories(${SIMPLEAI_BINARY_DIR}/src/ai)
	file(GLOB_RECURSE SRC ${SIMPLEAI_SOURCE_DIR}/src/test/*.cc ${SIMPLEAI_SOURCE_DIR}/src/test/*.cpp ${SIMPLEAI_SOURCE_DIR}/src/test/*.h)

	if (SIMPLEAI_LUA)
		message(STATUS "Build tests with lua support")
	else()
		message(STATUS "Build tests without lua support")
		file(GLOB LUA_REMOVE LUATreeLoader*)
		list(REMOVE_ITEM SRC ${LUA_REMOVE})
	endif()

	if (SIMPLEAI_XML)
		message(STATUS "Build tests with XML support")
	else()
		message(STATUS "Build tests without XML support")
		file(GLOB XML_REMOVE XMLTreeLoader*)
		list(REMOVE_ITEM SRC ${XML_REMOVE})
	endif()

	add_executable(tests ${SRC})
	target_link_libraries(tests simpleai ${CMAKE_THREAD_LIBS_INIT} ${GOOGLE_PROFILER_LIB} ${GOOGLE_TCMALLOC_LIB})

	if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
	elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
		target_link_libraries(tests -latomic)
	endif()
endif()

