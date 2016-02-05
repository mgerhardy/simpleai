include_directories(${SIMPLEAI_SOURCE_DIR}/src/ai ${SIMPLEAI_SOURCE_DIR}/src/test/gtest ${SIMPLEAI_SOURCE_DIR}/src/test/gtest/include)
link_directories(${SIMPLEAI_BINARY_DIR}/src/ai)
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
	ZoneTest.cpp ZoneTest.h
)

if (SIMPLEAI_LUA)
	list(APPEND SRC LUATreeLoaderTest.cpp LUATreeLoaderTest.h)
endif()

if (SIMPLEAI_XML)
	list(APPEND SRC XMLTreeLoaderTest.cpp XMLTreeLoaderTest.h)
endif()

add_executable(tests ${SRC})
target_link_libraries(tests simpleai)
