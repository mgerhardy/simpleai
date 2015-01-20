bin_PROGRAMS = \
	tests

AM_CPPFLAGS =

tests_SOURCES = \
	TestAll.cpp \
	TestShared.cpp \
	ParserTest.cpp \
	NodeTest.cpp \
	MessageTest.cpp \
	MovementTest.cpp \
	GeneralTest.cpp \
	AggroTest.cpp \
	GroupTest.cpp \
	\
	gtest/src/gtest.cc \
	gtest/src/gtest-death-test.cc \
	gtest/src/gtest-filepath.cc \
	gtest/src/gtest-port.cc \
	gtest/src/gtest-printers.cc \
	gtest/src/gtest-test-part.cc \
	gtest/src/gtest-typed-test.cc

if AI_ENABLE_LUA
AM_CPPFLAGS += -I$(top_builddir)/src/libs/lua
tests_SOURCES += \
	LUATreeLoaderTest.cpp
endif

if AI_ENABLE_XML
AM_CPPFLAGS += -I$(top_builddir)/src/libs/xml
tests_SOURCES += \
	XMLTreeLoaderTest.cpp
endif

tests_LDADD = \
	../ai/libsimpleai.la

tests_CPPFLAGS = @PTHREAD_CFLAGS@ -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai -I$(top_builddir)/src/test/gtest/include -I$(top_builddir)/src/test/gtest
tests_LDFLAGS = @PTHREAD_LIBS@
