noinst_HEADERS = \
	lapi.h \
	lauxlib.h \
	lcode.h \
	lctype.h \
	ldebug.h \
	ldo.h \
	lfunc.h \
	lgc.h \
	llex.h \
	llimits.h \
	lmem.h \
	lobject.h \
	lopcodes.h \
	lparser.h \
	lstate.h \
	lstring.h \
	ltable.h \
	ltm.h \
	luaconf.h \
	lua.h \
	lualib.h \
	lundump.h \
	lvm.h \
	lzio.h \
	lprefix.h

noinst_LTLIBRARIES = \
	liblua.la

liblua_la_SOURCES = \
	lapi.c \
	lauxlib.c \
	lbaselib.c \
	lbitlib.c \
	lcode.c \
	lcorolib.c \
	lctype.c \
	ldblib.c \
	ldebug.c \
	ldo.c \
	ldump.c \
	lfunc.c \
	lgc.c \
	linit.c \
	liolib.c \
	llex.c \
	lmathlib.c \
	lmem.c \
	loadlib.c \
	lobject.c \
	lopcodes.c \
	loslib.c \
	lparser.c \
	lstate.c \
	lstring.c \
	lstrlib.c \
	ltable.c \
	ltablib.c \
	ltm.c \
	lua.c \
	lundump.c \
	lvm.c \
	lzio.c \
	lutf8lib.c

liblua_la_CFLAGS = -I.

if !AI_EXCEPTIONS
liblua_la_CFLAGS += -DLUA_USE_LONGJMP
endif

liblua_la_LDFLAGS =

liblua_la_LIBADD =
