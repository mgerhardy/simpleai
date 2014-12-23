bin_PROGRAMS = \
	simpleai-run

simpleai_run_SOURCES = \
	Example.cpp

simpleai_run_LDADD = \
	../ai/libsimpleai.la

include_HEADERS = \
	actions/ExampleTask.h \
	GameEntity.h \
	GameMap.h

simpleai_run_CXXFLAGS = @PTHREAD_CFLAGS@ $(AM_CXXFLAGS) -Wall -pedantic -Wextra -Wabi -Wcast-qual -Wconversion -Wfloat-equal -Wmissing-declarations -Wmissing-format-attribute -Wno-long-long -Woverloaded-virtual -Wpacked -Wredundant-decls -Wshadow -Wsign-promo -Wno-sign-compare
simpleai_run_CXXFLAGS += -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai

simpleai_run_LDADD += @PTHREAD_LIBS@

EXTRA_DIST = behaviours.lua

AM_CPPFLAGS = -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai
