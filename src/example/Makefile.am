bin_PROGRAMS = \
	example

example_SOURCES = \
	Example.cpp \
	Pathfinder.cpp

example_LDADD = \
	../ai/libsimpleai.la

example_CPPFLAGS = -Wall

include_HEADERS = \
	actions/Move.h \
	actions/ExampleTask.h \
	GameEntity.h \
	GameMap.h \
	Pathfinder.h

EXTRA_DIST = behaviours.lua

INCLUDES = -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai
