set(SRCS
	lapi.c
	lauxlib.c
	lbaselib.c
	lbitlib.c
	lcode.c
	lcorolib.c
	lctype.c
	ldblib.c
	ldebug.c
	ldo.c
	ldump.c
	lfunc.c
	lgc.c
	linit.c
	liolib.c
	llex.c
	lmathlib.c
	lmem.c
	loadlib.c
	lobject.c
	lopcodes.c
	loslib.c
	lparser.c
	lstate.c
	lstring.c
	lstrlib.c
	ltable.c
	ltablib.c
	ltm.c
	lundump.c
	lutf8lib.c
	lvm.c
	lzio.c
	)
add_library(lua ${SRCS})
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	add_definitions("-DLUA_USE_LINUX")
endif()
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
#	add_definitions("-DLUA_USE_WINDOWS")
endif()
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
	add_definitions("-DLUA_USE_MACOSX")
endif()
if (CMAKE_DL_LIBS)
	target_link_libraries(lua ${CMAKE_DL_LIBS})
endif()
target_include_directories(simpleai PUBLIC .)
