#ifndef _TIMER_H_
#define _TIMER_H_

#include <unistd.h>
#include <stdio.h>
#include <sys/timerfd.h>

#include <string>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef struct timespec Timer_t;

/*static int create_timerfd() {
	int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0) {
		printf("Failed In timerfd_create\n");
  	}
  	return timerfd;
}*/

class Timer
{
public:
	Timer(int fd, bool only_once):timerfd_(fd), once_run_(only_once) {
		interval_.tv_sec = 0;
		interval_.tv_nsec = 0;
	}
	
	~Timer() {
		close(timerfd_);
	}

	int GetFd() const {
		return timerfd_;
	}

	int SetInterval(int sec, int msec);

	void GetInterval (int &sec, int &msec) const;

	int Countdown(int &sec, int &msec);

	int Disarm();

private:
	int timerfd_;
	bool once_run_;
	Timer_t interval_;	

};

// > >= == < <== Overload
inline bool operator>(const Timer_t &foo, const Timer_t &bar) {
	uint64_t sum_foo = 1000000000 * foo.tv_sec + foo.tv_nsec;
	uint64_t bar_foo = 1000000000 * bar.tv_sec + bar.tv_nsec;

	return (sum_foo > bar_foo);
}

inline bool operator>=(const Timer_t &foo, const Timer_t &bar) {
	uint64_t sum_foo = 1000000000 * foo.tv_sec + foo.tv_nsec;
	uint64_t bar_foo = 1000000000 * bar.tv_sec + bar.tv_nsec;
	
	return (sum_foo >= bar_foo);
}

inline bool operator==(const Timer_t &foo, const Timer_t &bar) {
	uint64_t sum_foo = 1000000000 * foo.tv_sec + foo.tv_nsec;
	uint64_t bar_foo = 1000000000 * bar.tv_sec + bar.tv_nsec;
	
	return (sum_foo == bar_foo);
}

inline bool operator<(const Timer_t &foo, const Timer_t &bar) {
	uint64_t sum_foo = 1000000000 * foo.tv_sec + foo.tv_nsec;
	uint64_t bar_foo = 1000000000 * bar.tv_sec + bar.tv_nsec;

	return (sum_foo < bar_foo);
}

inline bool operator<=(const Timer_t &foo, const Timer_t &bar) {
	uint64_t sum_foo = 1000000000 * foo.tv_sec + foo.tv_nsec;
	uint64_t bar_foo = 1000000000 * bar.tv_sec + bar.tv_nsec;

	return (sum_foo <= bar_foo);
}

#endif 
