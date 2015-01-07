#pragma once

#ifdef AI_NO_THREADING

#define MUTEXTYPE
#define MUTEX(name)
#define LOCK
#define SCOPEDLOCK(mutex)
#define ATOMIC(type) type
#define ATOMICINT int

#else

#include <thread>
#include <mutex>
#include <atomic>

#define MUTEXTYPE std::recursive_mutex
#define MUTEX(name) mutable MUTEXTYPE name
#define LOCK std::lock_guard<MUTEXTYPE>
#define SCOPEDLOCK(mutex) std::lock_guard<MUTEXTYPE> __lock__(mutex)
// TODO: use memory_order_release for store and memory_order_consume for load
#define ATOMIC(type) std::atomic<type>
#define ATOMICINT std::atomic_int

#endif
