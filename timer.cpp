#include "timer.h"

// Enable The Timer And Set Interval, Timer Will First Run After It's Specified Interval
int Timer::SetInterval(int sec, int msec) {
	struct itimerspec new_time; 		
	struct timespec current;

	clock_gettime(CLOCK_REALTIME, &current);	

	new_time.it_value.tv_sec = current.tv_sec + sec;
	new_time.it_value.tv_nsec = current.tv_nsec + 1000000 * msec;

	// If Nanosecond Field Exceeds 1000000000, Second Field Increase
	if (new_time.it_value.tv_nsec >= 1000000000) {
		new_time.it_value.tv_sec++;
		new_time.it_value.tv_nsec -= 1000000000;
	}

	if (once_run_) {
		new_time.it_interval.tv_sec = 0;
		new_time.it_interval.tv_nsec = 0;

	} else {
		new_time.it_interval.tv_sec = sec;
		new_time.it_interval.tv_nsec = 1000000 * msec;
	}

	interval_ = new_time.it_interval;

	return timerfd_settime(timerfd_, TFD_TIMER_ABSTIME, &new_time, NULL);
}


// When On Expiration, Run Callback Function, And Reset next_ member
int Timer::ActivateCb(void *data) {
	tick_times_++;	
	// Invoke Callback
	int ret = callback_(data);
	// Refresh The Remaining Amount Time Util Next Expiration
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
	return timerfd_settime(timerfd_, 0, &new_time, NULL);
}
