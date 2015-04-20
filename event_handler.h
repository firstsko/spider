#ifndef _EVENT_HANDLER_
#define _EVENT_HANDLER_

#include <string>

#include "event_driver.h"
#include "server.h"
#include "sock.h"

class EventHandler {
public:
	EventHandler():sk_(NULL) {

	}

	~EventHandler() {
		if (sk_ != NULL) {
			delete sk_;
		}
	}
	
	// As A Client Launch A Connection
	int OnConnect(const std::string &ip, int port, int timeout = 10);

	int SendRequest(void *message);

	int SendResponse(void *message);

	int SendMessage(void *message);

private:
	Socket *sk_;
};

#endif
