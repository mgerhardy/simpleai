#include "ThreadTest.h"

class ThreadTest: public TestSuite {
};

TEST_F(ThreadTest, testScheduleAtFixedRate) {
	ai::ThreadScheduler scheduler;
	std::atomic_int countExecution(0);
	const int delay = 10;
	const int waitTime = 200;
	scheduler.scheduleAtFixedRate(std::chrono::milliseconds(0), std::chrono::milliseconds(delay), [&] () {
		++countExecution;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	const int delta = 3;
	const int expected = waitTime / delay;
	ASSERT_GE(countExecution, expected - delta);
	ASSERT_LE(countExecution, expected + delta);
}

TEST_F(ThreadTest, testSchedule) {
	ai::ThreadScheduler scheduler;
	std::atomic_int countExecutionOnce(0);
	scheduler.schedule(std::chrono::milliseconds(0), [&] () {
		++countExecutionOnce;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ASSERT_EQ(1, countExecutionOnce);
}

TEST_F(ThreadTest, testScheduleInitialDelay) {
	ai::ThreadScheduler scheduler;
	std::atomic_int countExecutionOnce(0);
	scheduler.schedule(std::chrono::milliseconds(20), [&] () {
		++countExecutionOnce;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ASSERT_EQ(0, countExecutionOnce);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	ASSERT_EQ(1, countExecutionOnce);
}

TEST_F(ThreadTest, testScheduleAbortBeforeExecute) {
	ai::ThreadScheduler scheduler;
	std::atomic_int countExecutionOnce(0);
	const int timerId = scheduler.schedule(std::chrono::milliseconds(20), [&] () {
		++countExecutionOnce;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ASSERT_TRUE(scheduler.cancel(timerId));
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	ASSERT_EQ(0, countExecutionOnce);
}
