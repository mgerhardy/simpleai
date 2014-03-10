#pragma once

#define GCC_ATLEAST(major, minor) (defined __GNUC__ && (__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor))))

#ifndef __has_extension // clang feature detection
#	ifdef __has_feature
#		define __has_extension __has_feature
#	else
#		define __has_extension(x) 0
#	endif
#endif

#define CXX11(gcc_major, gcc_minor, msc_ver, clang_feature) ( \
	__cplusplus >= 201103L || \
	(defined __GXX_EXPERIMENTAL_CXX0X__ && GCC_ATLEAST((gcc_major), (gcc_minor))) || \
	(defined _MSC_VER && (msc_ver) != 0 && _MSC_VER >= (msc_ver)) || \
	__has_extension(clang_feature) \
)

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#define DEPRECATED __attribute__((__deprecated__))
#else
#define DEPRECATED
#endif

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define DEPRECATED_FOR(f) __attribute__((deprecated("Use " #f " instead")))
#else
#define DEPRECATED_FOR(f) DEPRECATED
#endif

#if not CXX11(4, 7, 1400, cxx_override_control)
#	define override
#endif

#if __cplusplus < 201103L
#ifndef nullptr
/* not typesafe as the real nullptr from c++11 */
#define nullptr 0
#endif
#endif

#if 0
#ifdef DEBUG
#define __forceinline inline
#else
#define __forceinline inline __attribute__((always_inline))
#endif
#endif

#if !defined(__GNUC__)
#define  __attribute__(x)
#endif

#ifndef __INTEL_COMPILER
# if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 402
#  define GCC_DIAG_STR(s) #s
#  define GCC_DIAG_JOINSTR(x,y) GCC_DIAG_STR(x ## y)
#  define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
#  define GCC_DIAG_PRAGMA(x) GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
#  if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
#   define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(push) GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#   define GCC_DIAG_ON(x) GCC_DIAG_PRAGMA(pop)
#  else
#   define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#   define GCC_DIAG_ON(x)  GCC_DIAG_PRAGMA(warning GCC_DIAG_JOINSTR(-W,x))
#  endif
# else
#  define GCC_DIAG_OFF(x)
#  define GCC_DIAG_ON(x)
# endif
#else
# define GCC_DIAG_OFF(x)
# define GCC_DIAG_ON(x)
#endif
