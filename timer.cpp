#include "timer.h"

// Enable The Timer And Set Interval, Timer Will First Run After It's Specified Interval
int Timer::SetInterval(int sec, int msec) {
	struct itimerspec new_time; 		
	struct timespec current;

	clock_gettime(CLOCK_MONOTONIC, &current);	

	if (once_run_) {
		new_time.it_interval.tv_sec = sec;
		new_time.it_interval.tv_nsec = 1000000 * sec;
	} else {
		new_time.it_interval.tv_sec = sec;
		new_time.it_interval.tv_nsec = 1000000 * sec;
		new_time.it_value.tv_sec = current.tv_sec + sec;
		new_time.it_value.tv_nsec = current.tv_nsec + 1000000 * msec;
	}

	interval_ = new_time.it_interval;

	return timerfd_settime(timerfd_, TFD_TIMER_ABSTIME, &new_time, NULL);
}

// The Remaining Amount Time Util Next Expiration
int Timer::Countdown(int &sec, int &msec) {
	struct itimerspec next; 		
	int ret = timerfd_gettime(timerfd_, &next);
	if (ret == 0) {
		sec = next.it_value.tv_sec;
		msec = next.it_value.tv_nsec/100000;
	}
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
