bin_PROGRAMS = \
	simpleai-tests

AM_CPPFLAGS =

simpleai_tests_SOURCES = \
	AggroTest.cpp \
	GeneralTest.cpp \
	GroupTest.cpp \
	MessageTest.cpp \
	MovementTest.cpp \
	NodeTest.cpp \
	ParserTest.cpp \
	TestAll.cpp \
	TestShared.cpp \
	ThreadTest.cpp \
	ZoneTest.cpp \
	\
	gtest/src/gtest.cc \
	gtest/src/gtest-death-test.cc \
	gtest/src/gtest-filepath.cc \
	gtest/src/gtest-port.cc \
	gtest/src/gtest-printers.cc \
	gtest/src/gtest-test-part.cc \
	gtest/src/gtest-typed-test.cc

EXTRA_DIST = luaregistry.lua

simpleai_tests_CPPFLAGS = @PTHREAD_CFLAGS@ -I$(top_srcdir)/src/libs -I$(top_srcdir)/src/libs/lua -I$(top_srcdir)/src/libs/tinyxml2 -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai -I$(top_builddir)/src/test/gtest/include -I$(top_builddir)/src/test/gtest
simpleai_tests_LDFLAGS = @PTHREAD_LIBS@
simpleai_tests_LDADD = ../ai/libsimpleai.la @MATH_LIBS@

if AI_ENABLE_LUA
simpleai_tests_SOURCES += \
	LUATreeLoaderTest.cpp
simpleai_tests_CPPFLAGS += \
	-DAI_INCLUDE_LUA
endif

if AI_ENABLE_XML
simpleai_tests_SOURCES += \
	XMLTreeLoaderTest.cpp
simpleai_tests_CPPFLAGS += \
	-DAI_INCLUDE_XML
endif
