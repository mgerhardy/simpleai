#pragma once

#ifndef AI_NO_THREADING

#include <thread>
#include <mutex>
#include <atomic>

#define MUTEXTYPE std::recursive_mutex
#define MUTEX(name) mutable MUTEXTYPE name
#define LOCK std::lock_guard<MUTEXTYPE>
#define SCOPEDLOCK(mutex) std::lock_guard<MUTEXTYPE> __lock__(mutex)
#define ATOMIC(type) std::atomic<type>

#else

#define MUTEXTYPE
#define MUTEX(name)
#define LOCK
#define SCOPEDLOCK(mutex)
#define ATOMIC(type) type

#endif
