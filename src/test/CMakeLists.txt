enable_testing()

include_directories(
	${SIMPLEAI_SOURCE_DIR}/src/ai
	${SIMPLEAI_SOURCE_DIR}/src/test/gtest ${SIMPLEAI_SOURCE_DIR}/src/test/gtest/include
)
set(SRC
	AggroTest.cpp AggroTest.h
	GeneralTest.cpp GeneralTest.h
	GroupTest.cpp GroupTest.h
	MessageTest.cpp MessageTest.h
	MovementTest.cpp MovementTest.h
	NodeTest.cpp NodeTest.h
	ParserTest.cpp ParserTest.h
	TestAll.cpp
	TestEntity.h
	TestShared.cpp TestShared.h
	ThreadTest.cpp ThreadTest.h
	XMLTreeLoaderTest.cpp XMLTreeLoaderTest.h
	ZoneTest.cpp ZoneTest.h

	gtest/src/gtest.cc
	gtest/src/gtest-death-test.cc
	gtest/src/gtest-filepath.cc
	gtest/src/gtest-port.cc
	gtest/src/gtest-printers.cc
	gtest/src/gtest-test-part.cc
	gtest/src/gtest-typed-test.cc
)

set(SIMPLEAI_TESTS_DEFINITIONS)
if (SIMPLEAI_LUA)
	list(APPEND SIMPLEAI_TESTS_DEFINITIONS AI_INCLUDE_LUA)
	list(APPEND SRC LUATreeLoaderTest.cpp LUATreeLoaderTest.h)
	list(APPEND SRC LUAAIRegistryTest.cpp LUAAIRegistryTest.h)
endif()

list(APPEND SIMPLEAI_TESTS_DEFINITIONS AI_INCLUDE_XML)

add_executable(simpleai-tests ${SRC})
if (SIMPLEAI_TESTS_DEFINITIONS)
	target_compile_definitions(simpleai-tests PRIVATE ${SIMPLEAI_TESTS_DEFINITIONS})
endif()
target_include_directories(simpleai-tests PUBLIC ${SIMPLEAI_SOURCE_DIR}/src/libs/tinyxml2/)
target_link_libraries(simpleai-tests simpleai tinyxml2)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

check_lua_files(simpleai-tests luaregistry.lua)
