AC_INIT(simpleai, 0.1, mgerhardy@github)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADER([src/ai/Config.h])

AM_INIT_AUTOMAKE([foreign])
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

AM_CXXFLAGS="-std=c++11"
AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE(debug,AS_HELP_STRING([--enable-debug=@<:@no/yes@:>@],[enable debug data generation]),,[enable_debug=no])
if test "x$enable_debug" != "xno" ; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    AM_CXXFLAGS="-g -Wall -Wextra -Werror -O0 $AM_CXXFLAGS"
else
    AC_DEFINE([NDEBUG],[],[No-debug Mode])
    AM_CXXFLAGS="-O3 -ftree-vectorize -msse3 -Wall -Wextra $AM_CXXFLAGS"
fi
AC_MSG_RESULT([$enable_debug])

AC_MSG_CHECKING([whether to build with gprof support])
AC_ARG_ENABLE(gprof,AS_HELP_STRING([--enable-gprof=@<:@no/yes@:>@],[build with support for gprof]),,[enable_gprof=no])
if test "x$enable_gprof" != "xno" ; then
	AM_CXXFLAGS="-pg $AM_CXXFLAGS"
	AM_LDFLAGS="-pg $AM_LDFLAGS"
fi
AC_MSG_RESULT([$enable_gprof])

AC_MSG_CHECKING([whether to build with google profiler support])
AC_ARG_ENABLE(google_profiler,AS_HELP_STRING([--enable-google-profiler=@<:@no/yes@:>@],[build with support for google profiler]),,[enable_google_profiler=no])
if test "x$enable_google_profiler" != "xno" ; then
	AM_CXXFLAGS="-DAI_PROFILER $AM_CXXFLAGS"
	AM_LDFLAGS="-profiler -tcmalloc $AM_LDFLAGS"
fi
AC_MSG_RESULT([$enable_google_profiler])

AC_MSG_CHECKING([whether to build with lua support])
AC_ARG_ENABLE(lua,AS_HELP_STRING([--enable-lua=@<:@no/yes@:>@],[build with support for lua]),,[enable_lua=yes])
if test "x$enable_lua" = "xyes" ; then
	AC_SUBST([AI_ENABLE_LUA],1)
	EXTERNAL_DEPENDENCIES="lua $EXTERNAL_DEPENDENCIES"
else
	AC_SUBST([AI_ENABLE_LUA],0)
fi
AM_CONDITIONAL(AI_ENABLE_LUA,test "x$enable_lua" = "xyes")
AC_MSG_RESULT([$enable_lua])

AX_PTHREAD

if test "x$enable_lua" != "xno" ; then
	AC_MSG_CHECKING([whether to build the run tool])
	AC_ARG_ENABLE(run,AS_HELP_STRING([--enable-run=@<:@no/yes@:>@],[build the run project]),,[enable_run=no])
	AC_MSG_RESULT([$enable_run])
	if test "x$enable_run" = "xno" ; then
		AC_SUBST([AI_NO_RUN],1)
	else
		MODULES="run $MODULES"
	fi
else
	enable_run=no
	AC_MSG_NOTICE([skipping run because it needs lua])
	AC_SUBST([AI_NO_RUN],1)
fi

AC_MSG_CHECKING([whether to build the tests])
AC_ARG_ENABLE(tests,AS_HELP_STRING([--enable-tests=@<:@no/yes@:>@],[build the tests]),,[enable_tests=no])
AC_MSG_RESULT([$enable_tests])
if test "x$enable_tests" = "xno" ; then
	AC_SUBST([AI_NO_TESTS],1)
else
	MODULES="test $MODULES"
fi

OLD_LIBS=$LIBS
LIBS=
AC_CHECK_LIB(m, sqrt)
MATH_LIBS=$LIBS
LIBS=$OLD_LIBS

AC_CHECK_LIB(atomic, __atomic_store)

AC_SUBST([EXTERNAL_DEPENDENCIES])
AC_SUBST([MODULES])
AC_SUBST([MATH_LIBS])
AC_SUBST([AM_LDFLAGS])
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([PTHREAD_LIBS])
AC_SUBST([PTHREAD_CFLAGS])

LT_INIT

AC_OUTPUT(
	Makefile
	src/Makefile
	src/ai/Makefile
	src/run/Makefile
	src/test/Makefile
	src/libs/Makefile
	src/libs/lua/Makefile
	docs/Makefile
	libsimpleai.pc
)

AC_MSG_RESULT([
simpleai $VERSION: Automatic configuration OK.

  Configuration summary:
    Platform: .......... $host
    Compiler: .......... $CXX
    C++ Flags: ......... $AM_CXXFLAGS $PTHREAD_CFLAGS
    Libraries: ......... $LIBS
    Linking: ........... $AM_LDFLAGS $PTHREAD_LIBS $MATH_LIBS
    Run: ............... $enable_run
    Tests: ............. $enable_tests
    LUA: ............... $enable_lua
    External: .......... $EXTERNAL_DEPENDENCIES
    Modules: ........... $MODULES

Type 'make' to build simpleai.  Type 'make install' to install it.
If you built the run tool, you can start it by executing 'simpleai-run'.
])
