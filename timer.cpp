#include "timer.h"

// Enable The Timer And Set Interval, Timer Will First Run After It's Specified Interval
int Timer::SetInterval(int sec, int msec) {
	struct itimerspec new_time; 		
	struct timespec current;

	clock_gettime(CLOCK_MONOTONIC, &current);	

	new_time.it_value.tv_sec = current.tv_sec + sec;
	new_time.it_value.tv_nsec = current.tv_nsec + 1000000 * msec;

	// If Nanosecond Field Exceeds 1000000000, Second Field Increase
	if (new_time.it_value.tv_nsec > 1000000000) {
		new_time.it_value.tv_sec++;
	}

	if (once_run_) {
		new_time.it_interval.tv_sec = 0;
		new_time.it_interval.tv_nsec = 0;

	} else {
		new_time.it_interval.tv_sec = sec;
		new_time.it_interval.tv_nsec = 1000000 * msec;
	}

	next_.tv_sec = current.tv_sec + interval_.tv_sec;
	next_.tv_nsec = current.tv_nsec + interval_.tv_nsec;

	interval_ = new_time.it_interval;

	return timerfd_settime(timerfd_, TFD_TIMER_ABSTIME, &new_time, NULL);
}

// Refresh The Remaining Amount Time Util Next Expiration
int Timer::Countdown() {
	// Unlikely Happen
	if (tick_times_ && once_run_) {
		next_.tv_sec = 0;
		next_.tv_nsec = 0;
		return 0;
	}

	struct timespec current;
	struct itimerspec next;
	int ret = clock_gettime(CLOCK_REALTIME, &current);	
	if (ret != 0) {
		return ret;
	}

	ret = timerfd_gettime(timerfd_, &next);
	if (ret != 0) {
		return ret;
	}

	next_.tv_sec = current.tv_sec + next.it_value.tv_sec;
	next_.tv_nsec = current.tv_nsec + next.it_value.tv_nsec;

	return ret;
}

// When On Expiration, Run Callback Function, And Reset next_ member
int Timer::ActiveCb(void *data) {
	tick_times_++;
	// Invoke Callback
	int ret = callback_(data);
	// Refresh The Remaining Amount Time Util Next Expiration
	Countdown();
	return ret;
}

// The Interval
void Timer::GetInterval(int &sec, int &msec) const {
	sec = interval_.tv_sec;
	msec = interval_.tv_nsec/100000;
}

// Turn Off The Timer (May Be Temporarily)
int Timer::Disarm() {
	struct itimerspec new_time = { {0, 0}, {0, 0} };		
	return timerfd_settime(timerfd_, TFD_TIMER_ABSTIME, &new_time, NULL);
}
