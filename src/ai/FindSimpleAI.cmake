# - Try to find SimpleAI
# Once done this will define
#  SIMPLEAI_FOUND - System has SimpleAI
#  SIMPLEAI_INCLUDE_DIRS - The SimpleAI include directories
#  SIMPLEAI_LIBRARIES - The libraries needed to use SimpleAI
#  SIMPLEAI_DEFINITIONS - Compiler switches required for using SimpleAI

find_package(PkgConfig)
pkg_check_modules(PC_SIMPLEAI QUIET libsimpleai)
set(SIMPLEAI_DEFINITIONS ${PC_SIMPLEAI_CFLAGS_OTHER})

find_path(SIMPLEAI_INCLUDE_DIR SimpleAI.h
          HINTS ${PC_SIMPLEAI_INCLUDEDIR} ${PC_SIMPLEAI_INCLUDE_DIRS}
          PATH_SUFFIXES simpleai)

find_library(SIMPLEAI_LIBRARY NAMES simpleai
             HINTS ${PC_SIMPLEAI_LIBDIR} ${PC_SIMPLEAI_LIBRARY_DIRS})

set(SIMPLEAI_LIBRARIES ${SIMPLEAI_LIBRARY})
set(SIMPLEAI_INCLUDE_DIRS ${SIMPLEAI_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SIMPLEAI_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(SimpleAI DEFAULT_MSG SIMPLEAI_LIBRARY SIMPLEAI_INCLUDE_DIR)

mark_as_advanced(SIMPLEAI_INCLUDE_DIRS SIMPLEAI_INCLUDE_DIR SIMPLEAI_LIBRARY)
