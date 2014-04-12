AC_INIT(simpleai, 0.1, mgerhardy@github)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADER([src/config.h])

AM_INIT_AUTOMAKE
AC_SUBST(VERSION)

# store current user given compiler flags to avoid default setup via AC_PROG_CXX
OLD_CXXFLAGS=$CXXFLAGS

AC_PROG_CXXCPP
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_HEADER_STDC
AC_CONFIG_MACRO_DIR([m4])

AC_MSG_CHECKING([Compiling with clang])
AC_COMPILE_IFELSE(
[AC_LANG_PROGRAM([], [[
#ifndef __clang__
#error "NO CLANG"
#endif
]])],
[CLANG=yes], [CLANG=no])
AC_MSG_RESULT([$CLANG])
if test "x$CLANG" = "xyes"; then
	#AM_CXXFLAGS="$AM_CXXFLAGS -Weverything -Wno-error"
	AM_CXXFLAGS="$AM_CXXFLAGS -std=c++11 -Wdocumentation"
fi

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi
AM_CONDITIONAL([HAVE_DOXYGEN],[test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])

# reset compiler flags to initial flags
CXXFLAGS=$OLD_CXXFLAGS

AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug data generation (def=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    AM_CXXFLAGS="$AM_CXXFLAGS -g -Wall -Werror -Wno-uninitialized -O0"
else
    AC_DEFINE([NDEBUG],[],[No-debug Mode])
    AM_CXXFLAGS="$AM_CXXFLAGS -O3"
fi

AC_ARG_ENABLE(gprof,AS_HELP_STRING([--enable-gprof=@<:@no/yes@:>@],[build with support for gprof]),,[enable_gprof=no])
if test "x$enable_gprof" != "xno" ; then
	AM_CXXFLAGS="$AM_CXXFLAGS -pg"
	LDFLAGS="$LDFLAGS -pg"
fi

# libm
AC_CHECK_LIB(m, sqrt,, [AC_MSG_ERROR([requires math library])])
MATH_LIBS=-lm
AC_SUBST(MATH_LIBS)

AC_SUBST(LDFLAGS)
AC_SUBST([AM_CXXFLAGS])

LT_INIT

AC_OUTPUT(
	Makefile
	src/Makefile
	src/ai/Makefile
	src/debug/src/Version.h
	src/example/Makefile
	src/test/Makefile
	src/libs/Makefile
	docs/Makefile
	libsimpleai.pc
)

AC_MSG_RESULT([
simpleai $VERSION: Automatic configuration OK.

  Configuration summary:
    Platform: .......... $host
    Compiler: .......... $CXX
    C++ Flags: ......... $CXXFLAGS
    Libraries: ......... $LIBS
    Linking: ........... $LDFLAGS

Type 'make' to build simpleai.  Type 'make install' to install it.
])

