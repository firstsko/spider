#ifndef _CHANNEL_
#define _CHANNEL_

#include <sys/time.h>
#include <string>
#include <string.h>

#include "event_driver.h"
#include "server.h"
#include "sock.h"

class Channel {
public:
	Channel(){

	}

	~Channel(){
	}
	
	// As A Client Launch A Connection
	int OnConnect(const std::string &ip, int port, int timeout = 10);

	int SendRequest(const std::string &ip, int port, void *message, size_t len);

	int SendResponse(void *message, size_t len);

	int SendMessage(void *message, size_t len);

private:
	void UpdateTime();

private:
	Socket* sk_;
	struct timeval tv_;
};

#endif
