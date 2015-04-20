#ifndef _CHANNEL_
#define _CHANNEL_

#include <string>

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
	int OnConnect(Socket *sk, const std::string &ip, int port, int timeout = 10);

	int SendRequest(Socket *sk, void *message, size_t len);

	int SendResponse(Socket *sk, void *message, size_t len);

	int SendMessage(Socket *sk, void *message, size_t len);

private:
};

#endif
