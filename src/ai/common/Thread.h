#pragma once

#ifndef AI_NO_THREADING

#include <thread>
#include <mutex>

#define MUTEXTYPE std::recursive_mutex
#define MUTEX(name) MUTEXTYPE name
#define LOCK std::lock_guard<MUTEXTYPE>
#define SCOPEDLOCK(mutex) std::lock_guard<MUTEXTYPE> __lock__(mutex)

#else

#define MUTEXTYPE
#define MUTEX(name)
#define LOCK
#define SCOPEDLOCK(mutex)

#endif
