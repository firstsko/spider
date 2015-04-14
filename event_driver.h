#ifndef _EVENT_DRIVER_
#define _EVENT_DRIVER_

#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <map>
#include <list>

#include "timer.h"
#include "sock.h"

#define MAX_EVENT_NO 1024

typedef enum {
	LEVEL_TRIGGER = 10,
	EDGE_TRIGGER,
} Trigger_t;

class EventDriver {
public:
	EventDriver():epfd_(0) {};

	~EventDriver();
	
	void CreateDriver();

	void AddEvent(int fd, Socket *sk, Trigger_t type = EDGE_TRIGGER);

	void ModifyEvent(int fd, int active_type);

	void DelEvent(int fd);

	void AddTimer(Timer *timer);

	void DelTimer(Timer *timer);

	void Tick(int fd);

	void StartLoop(int timeout_usec = 1000);
	
	static EventDriver* Instance();

private:
	void PrintErrno() {
		printf("%s %d Error %d Info:%s\n", __FILE__, __LINE__, errno, strerror(errno));
	}

private:
	int epfd_; 

	std::map <int, Socket *> event_container_;

	// We Need A Sorted Structure to Hold Timer Events, MinHeap
	std::list < Timer *> timer_container_;

private:
	static EventDriver* p_event_driver_;
};

#endif
