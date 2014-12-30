find_package(Threads)

option(SIMPLEAI_PROFILER "Use Google cpu profiler" OFF)

if (SIMPLEAI_PROFILER)
	FIND_LIBRARY(GOOGLE_PROFILER_LIB profiler)
	FIND_LIBRARY(GOOGLE_TCMALLOC_LIB tcmalloc)
	if (GOOGLE_PROFILER_LIB AND GOOGLE_TCMALLOC_LIB)
		message(STATUS "Found Google profiler: ${GOOGLE_PROFILER_LIB} ${GOOGLE_TCMALLOC_LIB}")
		add_definitions(-DAI_PROFILER)
	else()
		message(STATUS "Can't find Google profiler")
	endif()
endif()

if (SIMPLEAI_RUN)
	include_directories(${SIMPLEAI_SOURCE_DIR}/src/ai)
	link_directories(${SIMPLEAI_BINARY_DIR}/src/ai)
	file(GLOB_RECURSE SRC ${SIMPLEAI_SOURCE_DIR}/src/run/*.cpp ${SIMPLEAI_SOURCE_DIR}/src/run/*.h)
	add_executable(simpleai-run ${SRC})
	target_link_libraries(simpleai-run simpleai ${CMAKE_THREAD_LIBS_INIT} -latomic ${GOOGLE_PROFILER_LIB} ${GOOGLE_TCMALLOC_LIB})
endif()

