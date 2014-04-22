bin_PROGRAMS = \
	example

example_SOURCES = \
	Example.cpp \
	Pathfinder.cpp

example_LDADD = \
	../ai/libsimpleai.la

include_HEADERS = \
	actions/Move.h \
	actions/ExampleTask.h \
	conditions/IsMoving.h \
	GameEntity.h \
	GameMap.h \
	Pathfinder.h

example_CXXFLAGS = $(AM_CXXFLAGS) -Wall -pedantic -Wextra -Wabi -Wcast-qual -Wconversion -Wfloat-equal -Wmissing-declarations -Wmissing-format-attribute -Wno-long-long -Woverloaded-virtual -Wpacked -Wredundant-decls -Wshadow -Wsign-promo -Wno-sign-compare

EXTRA_DIST = behaviours.lua

AM_CPPFLAGS = -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai
