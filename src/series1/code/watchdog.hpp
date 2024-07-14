/*
https://github.com/prodeveloper0/WatchDogTimer
*/

#pragma once
#ifndef WATCHDOGTIMER_CLASS_H
#define WATCHDOGTIMER_CLASS_H
#include <chrono>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <functional>

//#include "initial_settings.hpp"
#include "../code/parameters.hpp"
// Parameters global_parameters;


template<typename Clock>
class BaseWatchdogTimer
{
public:
	typedef std::chrono::milliseconds Milliseconds;
	
private:
	std::mutex mutex;
	std::condition_variable loop_condition;
	std::atomic_bool loop_flag;
	std::thread loop_thread;

private:
	typename Clock::time_point clock_counter;

private:
	BaseWatchdogTimer(const BaseWatchdogTimer<Clock>&) = delete;
	BaseWatchdogTimer(BaseWatchdogTimer<Clock>&&) = delete;
	BaseWatchdogTimer<Clock>& operator=(const BaseWatchdogTimer<Clock>&) = delete;
	BaseWatchdogTimer<Clock>& operator=(BaseWatchdogTimer<Clock>&&) = delete;

private:
	// void forever(Milliseconds timeout, bool loop)
	// {
	// 	std::unique_lock<std::mutex> lock(this->mutex);

	// 	do
	// 	{
	// 		do
	// 		{
	// 			// 'std::condition_variable::wait_for' may return no_timeout although time exceeds timeout
	// 			// So, must use another variable like 'loop_flag'
	// 			if (this->loop_condition.wait_for(lock, timeout, [=]() {return !this->loop_flag.load(); }))
	// 			{
	// 				if (!this->loop_flag)
	// 					goto out;
	// 			}
	// 		} while (std::chrono::duration_cast<Milliseconds>((Clock::now() - this->clock_counter)) < timeout);
	// 		this->on_timeout();
	// 	} while (loop);

	// out:
	// 	return;
	// }
	void forever(std::chrono::milliseconds timeout, bool loop) {
		std::unique_lock<std::mutex> lock(mutex);

		// Use structured binding for clarity and potential performance gain
		auto const now = Clock::now();
		auto const wait_duration = timeout - std::chrono::duration_cast<std::chrono::milliseconds>(now - clock_counter);

		do {
			if (loop_condition.wait_for(lock, wait_duration, [=]() -> bool {
				// Lambda capture by copy for loop_flag
				return !loop_flag.load();
			})) {
			if (!loop_flag) {
				return; // Exit the loop directly
			}
			} else {
			// Handle timeout explicitly if wait_for doesn't return no_timeout
			on_timeout();
			}
		} while (loop);
	}

public:
	std::function<void(void)> on_timeout_callback;

private:
	virtual void on_timeout()
	{
		if (on_timeout_callback)
			on_timeout_callback();
	}

public:
	BaseWatchdogTimer()
	{

	}

	virtual ~BaseWatchdogTimer()
	{
		stop();
	}

public:
	void kick(unsigned int timeout_ms, bool loop = false)
	{
		kick(Milliseconds(timeout_ms), loop);
	}

	void kick(Milliseconds timeout, bool loop = false)
	{
		stop();

		loop_flag = true;
		loop_thread = std::thread(std::bind(&BaseWatchdogTimer<Clock>::forever, this, timeout, loop));
	}

	void clear()
	{
		this->clock_counter = Clock::now();
	}

	void stop()
	{
		loop_flag = false;
		loop_condition.notify_all();

		if (loop_thread.joinable() && loop_thread.get_id() != std::this_thread::get_id())
			loop_thread.join();
	}
};


typedef std::chrono::high_resolution_clock HighResolutionClock;
typedef std::chrono::system_clock SystemClock;
typedef HighResolutionClock DefaultClock;

typedef BaseWatchdogTimer<HighResolutionClock> HighResolutionWatchdogTimer;
typedef BaseWatchdogTimer<SystemClock> SystemClockWatchdogTimer;
typedef BaseWatchdogTimer<DefaultClock> WatchdogTimer;

class MyWatchdogTimer : public WatchdogTimer
{
public:
  virtual void on_timeout()
  {
    // To do when your dog is bark.
    global_parameters.global_abort_for_timeout = true;
  }
};
#endif
