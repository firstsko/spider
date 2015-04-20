#ifndef _CHANNEL_
#define _CHANNEL_

#include <string>

#include "event_driver.h"
#include "server.h"
#include "sock.h"

class Channel {
public:
	Channel():sk_(NULL) {

	}

	~Channel() {
		if (sk_ != NULL) {
			delete sk_;
		}
	}
	
	// As A Client Launch A Connection
	int OnConnect(const std::string &ip, int port, int timeout = 10);

	int SendRequest(void *message, size_t len);

	int SendResponse(void *message, size_t len);

	int SendMessage(void *message, size_t len);

private:
	Socket *sk_;
};

#endif
