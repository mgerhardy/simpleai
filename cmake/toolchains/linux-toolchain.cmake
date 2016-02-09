include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckCCompilerFlag)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)

# thread sanitizer doesn't work in combination with address and leak
set(CMAKE_REQUIRED_FLAGS "-Werror -fsanitize=thread")
check_c_compiler_flag("-fsanitize=thread" HAVE_FLAG_SANITIZE_THREAD)
set(CMAKE_REQUIRED_FLAGS "-Werror -fsanitize=undefined")
check_c_compiler_flag("-fsanitize=undefined" HAVE_FLAG_SANITIZE_UNDEFINED)
#set(CMAKE_REQUIRED_FLAGS "-Werror -fsanitize=address")
#check_c_compiler_flag("-fsanitize=address" HAVE_FLAG_SANITIZE_ADDRESS)
#set(CMAKE_REQUIRED_FLAGS "-Werror -fsanitize=leak")
#check_c_compiler_flag("-fsanitize=leak" HAVE_FLAG_SANITIZE_LEAK)
set(CMAKE_REQUIRED_FLAGS "-Werror -fexpensive-optimizations")
check_c_compiler_flag("-fexpensive-optimizations" HAVE_EXPENSIVE_OPTIMIZATIONS)
unset(CMAKE_REQUIRED_FLAGS)

#-Wthread-safety - http://clang.llvm.org/docs/ThreadSafetyAnalysis.html

if (HAVE_FLAG_SANITIZE_UNDEFINED)
	set(SANITIZE_FLAGS "${SANITIZE_FLAGS} -fsanitize=undefined")
	message("Support undefined sanitizer")
endif()

if (HAVE_FLAG_SANITIZE_LEAK)
	set(SANITIZE_FLAGS "${SANITIZE_FLAGS} -fsanitize=leak")
	message("Support leak sanitizer")
endif()

if (HAVE_FLAG_SANITIZE_THREAD)
	set(SANITIZE_FLAGS "${SANITIZE_FLAGS} -fsanitize=thread")
	message("Support thread sanitizer")
endif()

if (HAVE_FLAG_SANITIZE_ADDRESS)
	set(SANITIZE_FLAGS "${SANITIZE_FLAGS} -fsanitize=address")
	message("Support address sanitizer")
endif()


if (CMAKE_COMPILER_IS_GNUCXX)
	check_function_exists(__atomic_fetch_add_4 HAVE___ATOMIC_FETCH_ADD_4)
	if (NOT HAVE___ATOMIC_FETCH_ADD_4)
		check_library_exists(atomic __atomic_fetch_add_4 "" HAVE_LIBATOMIC)
		if (HAVE_LIBATOMIC)
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -latomic")
		endif()
	endif()
endif()

set(CMAKE_C_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES} ${CMAKE_DL_LIBS}")

check_library_exists(rt clock_gettime "time.h" HAVE_CLOCKGETTIME)
if (HAVE_CLOCKGETTIME)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lrt")
	set(CMAKE_C_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES} -lrt")
	set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lrt")
else()
	# might also be in libc
	check_library_exists(c clock_gettime "time.h" HAVE_CLOCKGETTIME)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wcast-qual -Wcast-align -Wpointer-arith -Wno-long-long -Wno-multichar -Wshadow -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wreturn-type -Wwrite-strings -Wno-variadic-macros -Wno-unknown-pragmas")
if (CMAKE_USE_PTHREADS_INIT)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
endif()
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_GNU_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_FORTIFY_SOURCE=2 -DNDEBUG -fomit-frame-pointer -O3")
if (HAVE_EXPENSIVE_OPTIMIZATIONS)
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fexpensive-optimizations")
endif()
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -fno-omit-frame-pointer ${SANITIZE_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -Wnon-virtual-dtor")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_C_FLAGS_DEBUG}")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${SANITIZE_FLAGS}")
