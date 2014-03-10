noinst_HEADERS = \
	lua/lapi.h \
	lua/lauxlib.h \
	lua/lcode.h \
	lua/lctype.h \
	lua/ldebug.h \
	lua/ldo.h \
	lua/lfunc.h \
	lua/lgc.h \
	lua/llex.h \
	lua/llimits.h \
	lua/lmem.h \
	lua/lobject.h \
	lua/lopcodes.h \
	lua/lparser.h \
	lua/lstate.h \
	lua/lstring.h \
	lua/ltable.h \
	lua/ltm.h \
	lua/luaconf.h \
	lua/lua.h \
	lua/lualib.h \
	lua/lundump.h \
	lua/lvm.h \
	lua/lzio.h

noinst_LTLIBRARIES = \
	libexternal.la

libexternal_la_SOURCES = \
	lua/lapi.cpp \
	lua/lauxlib.cpp \
	lua/lbaselib.cpp \
	lua/lbitlib.cpp \
	lua/lcode.cpp \
	lua/lcorolib.cpp \
	lua/lctype.cpp \
	lua/ldblib.cpp \
	lua/ldebug.cpp \
	lua/ldo.cpp \
	lua/ldump.cpp \
	lua/lfunc.cpp \
	lua/lgc.cpp \
	lua/linit.cpp \
	lua/liolib.cpp \
	lua/llex.cpp \
	lua/lmathlib.cpp \
	lua/lmem.cpp \
	lua/loadlib.cpp \
	lua/lobject.cpp \
	lua/lopcodes.cpp \
	lua/loslib.cpp \
	lua/lparser.cpp \
	lua/lstate.cpp \
	lua/lstring.cpp \
	lua/lstrlib.cpp \
	lua/ltable.cpp \
	lua/ltablib.cpp \
	lua/ltm.cpp \
	lua/lua.cpp \
	lua/lundump.cpp \
	lua/lvm.cpp \
	lua/lzio.cpp

libexternal_la_CFLAGS = -DLUA_USE_LONGJMP

libexternal_la_LDFLAGS =
	
libexternal_la_LIBADD =