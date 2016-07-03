bin_PROGRAMS = \
	tests

AM_CPPFLAGS =

tests_SOURCES = \
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

if AI_ENABLE_LUA
AM_CPPFLAGS += -I$(top_srcdir)/src/libs/lua
tests_SOURCES += \
	LUATreeLoaderTest.cpp
endif

if AI_ENABLE_XML
AM_CPPFLAGS += -I$(top_srcdir)/src/libs/tinyxml2
tests_SOURCES += \
	XMLTreeLoaderTest.cpp
endif

tests_CPPFLAGS = @PTHREAD_CFLAGS@ -I$(top_srcdir)/src/libs -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai -I$(top_builddir)/src/test/gtest/include -I$(top_builddir)/src/test/gtest
tests_LDFLAGS = @PTHREAD_LIBS@
tests_LDADD = ../ai/libsimpleai.la @MATH_LIBS@
