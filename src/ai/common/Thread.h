#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#if AI_DEBUG_READWRITELOCK > 0
#include <chrono>
#include "Types.h"
#endif

namespace ai {

class ReadWriteLock {
private:
	mutable std::atomic_int _readers;
	mutable std::atomic_bool _lock;
	const std::string _name;
public:
	ReadWriteLock(const std::string& name) : _readers(0), _lock(false), _name(name) {}

	inline void lockRead() const {
#if AI_DEBUG_READWRITELOCK > 0
		auto start = std::chrono::system_clock::now();
#endif
		while (_lock) {
			std::this_thread::yield();
#if AI_DEBUG_READWRITELOCK > 0
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			ai_assert(diff.count() < AI_DEBUG_READWRITELOCK, "%s is blocked longer than %ims", _name.c_str(), AI_DEBUG_READWRITELOCK);
#endif
		}
		++_readers;
	}

	inline void unlockRead() const {
		--_readers;
	}

	inline void lockWrite() {
#if AI_DEBUG_READWRITELOCK > 0
		auto start = std::chrono::system_clock::now();
#endif
		while (std::atomic_exchange_explicit(&_lock, true, std::memory_order_acquire)) {
			std::this_thread::yield();
#if AI_DEBUG_READWRITELOCK > 0
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			ai_assert(diff.count() < AI_DEBUG_READWRITELOCK, "%s is blocked longer than %ims", _name.c_str(), AI_DEBUG_READWRITELOCK);
#endif
		}
		while (_readers > 0) {
			std::this_thread::yield();
#if AI_DEBUG_READWRITELOCK > 0
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			ai_assert(diff.count() < AI_DEBUG_READWRITELOCK, "%s is blocked longer than %ims", _name.c_str(), AI_DEBUG_READWRITELOCK);
#endif
		}
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
