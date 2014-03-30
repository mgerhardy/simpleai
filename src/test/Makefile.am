bin_PROGRAMS = \
	tests

tests_SOURCES = \
	TestAll.cpp \
	TestShared.cpp \
	ParserTest.cpp \
	NodeTest.cpp \
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

tests_LDADD = \
	../ai/libsimpleai.la

tests_CPPFLAGS =
tests_LDFLAGS = -pthread

include_HEADERS =

INCLUDES = -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai -I$(top_builddir)/src/test/gtest/include -I$(top_builddir)/src/test/gtest
