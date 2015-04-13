#ifndef _EVENT_DRIVER_
#define _EVENT_DRIVER_

#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <map>

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

	void DelEvent(int);

//	void AddTimer(int);

//	void DelTimer(int);

//	void Tick(int);

	void StartLoop(int timeout_usec = 1000);
	
	static EventDriver* Instance();

private:
	void PrintErrno() {
		printf("%s %d Error %d Info:%s\n", __FILE__, __LINE__, errno, strerror(errno));
	}

private:
	int epfd_; 

	std::map <int, Socket *> event_container_;

	// std::map <int > time_containr_;

	static EventDriver* p_event_driver_;
};

#endif
