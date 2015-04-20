#include "channel.h"
#include <arpa/inet.h>

using namespace std;

static char* iptostr(unsigned ip) {
	struct in_addr addr;
	memcpy(&addr, &ip, 4);
	return inet_ntoa(addr);
}

int CheckConnectTimeoutCb(void *data) {
	Socket *sk = (Socket *)data;
	if (sk->State() != SOCK_TCP_ENSTABLISHED) {
		ALERT("Connection To %s:%d Time Out", iptostr(sk->GetPeerAddr().sin_addr.s_addr), sk->GetPeerAddr().sin_port);
		delete sk;
		sk = NULL;
		return -1;
	}
	return 0;
}

// Connect With A Timeout
int Channel::OnConnect(Socket *sk, const string &ip,  int port, int timeout) {
	int ret = sk->Connect(ip, port, timeout);
	if (ret == 0) {
		INFO("Connection Complete Immediately");
		return 0;
	} else if (ret == SOCK_CONNECTTING) {
		// After Timeout, Check If The Socket Is Writable
		EventDriver::Instance()->AddTimer(timeout, 0, true, CheckConnectTimeoutCb, sk);
		return SOCK_CONNECTTING; 
	} else {
		return -1;
	}
}
	
int Channel::SendRequest(Socket *sk, void *message, size_t len) {


	return 0;
}

int Channel::SendResponse(Socket *sk, void *message, size_t len) {


	return 0;
}

// Not Doing Real Socket I/O Actions, Merely Append The Socket Object's output_ buff
int Channel::SendMessage(Socket *sk, void *message, size_t len) {


	return 0;
}