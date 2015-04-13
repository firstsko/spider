#ifndef _TIMER_H_
#define _TIMER_H_

#include <unistd.h>
#include <stdio.h>
#include <sys/timerfd.h>

#include <string>
#include <string.h>
#include <time.h>

int create_timerfd() {
	int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0) {
		printf("Failed In timerfd_create\n");
  	}
  	return timerfd;
}

class Timer
{
public:
	Timer(int fd, bool only_once):timerfd_(fd), once_run_(only_once) { }
	
	~Timer() {
		close(timerfd_);
	}

	int GetFd() {
		return timerfd_;
	}

	int SetInterval(int sec, int msec);

	int GetInterval(int &sec, int &msec);

	int Countdown(int &sec, int &msec);

	int Disarm();

private:
	int timerfd_;
	bool once_run_;
};

#endif 
