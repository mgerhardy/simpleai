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

namespace ai {

class ReadWriteLock {
private:
	mutable ATOMICINT _readers;
	mutable ATOMIC(bool) _lock;
public:
	ReadWriteLock() : _readers(0), _lock(false) {}

	inline void lockRead() const {
		while (_lock) {}
		++_readers;
	}

	inline void unlockRead() const {
		--_readers;
	}

	inline void lockWrite() {
		while (std::atomic_exchange_explicit(&_lock, true, std::memory_order_acquire)) {}
		while (_readers > 0) {}
	}

	inline void unlockWrite() {
		std::atomic_store_explicit(&_lock, false, std::memory_order_release);
	}
};

class ScopedReadLock {
private:
	const ReadWriteLock& _lock;
public:
	ScopedReadLock(const ReadWriteLock& lock) : _lock(lock) {
		_lock.lockRead();
	}
	~ScopedReadLock() {
		_lock.unlockRead();
	}
};

class ScopedWriteLock {
private:
	ReadWriteLock& _lock;
public:
	ScopedWriteLock(ReadWriteLock& lock) : _lock(lock) {
		_lock.lockWrite();
	}
	~ScopedWriteLock() {
		_lock.unlockWrite();
	}
};

}
