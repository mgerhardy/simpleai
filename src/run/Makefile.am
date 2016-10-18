bin_PROGRAMS = \
	simpleai-run

simpleai_run_SOURCES = \
	Main.cpp \
	GameEntity.cpp

simpleai_run_LDADD = \
	../ai/libsimpleai.la

include_HEADERS = \
	GameEntity.h \
	GameMap.h

simpleai_run_CXXFLAGS = @PTHREAD_CFLAGS@ $(AM_CXXFLAGS) -Wall -pedantic -Wextra -Wabi -Wcast-qual -Wconversion -Wfloat-equal -Wmissing-declarations -Wmissing-format-attribute -Wno-long-long -Woverloaded-virtual -Wpacked -Wredundant-decls -Wshadow -Wsign-promo -Wno-sign-compare
simpleai_run_CXXFLAGS += -I$(top_srcdir)/src/libs -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai
simpleai_run_LDADD += @PTHREAD_LIBS@

if AI_ENABLE_LUA
simpleai_run_CXXFLAGS += \
	-DAI_INCLUDE_LUA
simpleai_run_CXXFLAGS += -I$(top_srcdir)/src/libs/lua
endif

if AI_ENABLE_XML
simpleai_run_CXXFLAGS += \
	-DAI_INCLUDE_XML
simpleai_run_CXXFLAGS += -I$(top_srcdir)/src/libs/tinyxml2
endif

EXTRA_DIST = behaviours.lua

AM_CXXFLAGS = -I$(top_srcdir)/src/ai -I$(top_builddir)/src/ai
