#pragma once

#include "Types.h"
#include <thread>
#include <mutex>
#include <atomic>

namespace ai {

// TODO: not a real read-write-lock - maybe someday
class ReadWriteLock {
private:
	const std::string _name;
	mutable std::recursive_mutex _mutex;
public:
	ReadWriteLock(const std::string& name) :
			_name(name) {
	}

	inline void lockRead() const {
		_mutex.lock();
	}

	inline void unlockRead() const {
		_mutex.unlock();
	}

	inline void lockWrite() {
		_mutex.lock();
	}

	inline void unlockWrite() {
		_mutex.unlock();
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
