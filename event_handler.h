#ifndef _EVENT_HANDLER_
#define _EVENT_HANDLER_

#include <string>

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
	
	// When As A Client Launch A Connection
	int OnConnect(const std::string &ip, int port);

	// Close Connection With Peer
	int CloseConnect();
	
	int SendRequest();

	int SendResponse();

private:
	Socket *sk_;
};

#endif
