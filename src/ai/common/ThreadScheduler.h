#pragma once

#include <chrono>
#include <vector>
#include <functional>
#include <queue>
#include <thread>
#include <future>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace ai {

class ThreadScheduler {
private:
	struct ScheduledTask {
		ThreadScheduler* _scheduler;
		std::function<void()> _callback;
		std::chrono::milliseconds _initialDelay;
		std::chrono::milliseconds _delay;

		ScheduledTask(ThreadScheduler* scheduler, const std::function<void()>& callback, const std::chrono::milliseconds& initialDelay, const std::chrono::milliseconds& delay) :
				_scheduler(scheduler), _callback(callback), _initialDelay(initialDelay), _delay(delay) {
		}

		void operator()() const {
			_callback();
			// TODO: no need to reschedule in case the delay parameter is not valid
			//if (_delay <= 0)
			//	return;
			const ScheduledTask reschedule(_scheduler, _callback, _initialDelay + _delay, _delay);
			_scheduler->_tasks.emplace(reschedule);
		}

		bool operator<(const ScheduledTask& other) const {
			return other._initialDelay < _initialDelay;
		}
	};

	std::priority_queue<ScheduledTask, std::vector<ScheduledTask> > _tasks;
	std::condition_variable _condition;
	std::atomic_bool _stop;
	std::mutex _queueMutex;
	std::thread _thread;

public:
	ThreadScheduler() : _stop(false) {
		_thread = std::thread([this] {
			for (;;) {
				if (this->_tasks.empty()) {
					std::unique_lock<std::mutex> lock(this->_queueMutex);
					this->_condition.wait(lock, [this] {
						// TODO: return this->_stop;
						return false;
					});
					if (this->_stop) {
						return;
					}
				}
				auto epoch = std::chrono::system_clock::now().time_since_epoch();
				auto now = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
				while (this->_tasks.top()._initialDelay < now) {
					if (this->_stop) {
						return;
					}
					// TODO: use std::async here to not delay the tasks in this loop
					this->_tasks.top()();
					this->_tasks.pop();
				}
				if (this->_stop) {
					return;
				}
				std::this_thread::yield();
			}
		});
	}

	/**
	 * Will not wait for scheduled tasks
	 */
	~ThreadScheduler() {
		_stop = true;
		_condition.notify_one();
		_thread.join();
	}

	template<class F, class ... Args>
	void schedule(const std::chrono::milliseconds& delay, F&& f, Args&&... args) {
		scheduleAtFixedRate(delay, std::chrono::milliseconds(), f, args...);
	}

	/**
	 * @param[in] f The task to execute
	 * @param[in] initialDelay The milliseconds to delay the first execution
	 * @param[in] period The period in milliseconds between successive executions
	 */
	template<class F, class ... Args>
	void scheduleAtFixedRate(const std::chrono::milliseconds& initialDelay, const std::chrono::milliseconds& delay, F&& f, Args&&... args) {
		auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		auto epoch = std::chrono::system_clock::now().time_since_epoch();
		auto now = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
		std::unique_lock<std::mutex> lock(_queueMutex);
		_tasks.emplace(ScheduledTask(this, [task]() {task();}, now + initialDelay, delay));
		_condition.notify_one();
	}
};
}
