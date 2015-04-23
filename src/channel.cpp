#include <arpa/inet.h>

#include "channel.h"

using namespace std;

static char* iptostr(unsigned ip) {
	struct in_addr addr;
	memcpy(&addr, &ip, 4);
	return inet_ntoa(addr);
}

int CheckConnectTimeoutCb(void *data) {
	sk_ = (Socket *)data;
	if (sk_->State() != SOCK_TCP_ENSTABLISHED) {
		ALERT("Connection To %s:%d Time Out", iptostr(sk_->GetPeerAddr().sin_addr.s_addr), sk_->GetPeerAddr().sin_port);
		delete sk_;
		sk_ = NULL;
		return -1;
	}
	return 0;
}

// Connect With A Timeout
int Channel::OnConnect(const string &ip,  int port, int timeout) {
	int ret = sk_->Connect(ip, port, timeout);
	if (ret == 0) {
		INFO("Connection Complete Immediately");
		return 0;
	} else if (ret == SOCK_CONNECTTING) {
		// After Timeout, Check If The Socket Is Writable
		EventDriver::Instance()->AddTimer(timeout, 0, true, CheckConnectTimeoutCb, sk_);
		return SOCK_CONNECTTING; 
	} else {
		return -1;
	}
}
	
int Channel::SendRequest(void *message, size_t len) {

	return 0;
}

int Channel::SendResponse(void *message, size_t len) {

	return 0;
}

// Not Doing Real Socket I/O Actions, Merely Append The Socket Object's output_ buff
int Channel::SendMessage(void *message, size_t len) {
	memcpy(sk_->GetWriteIndex(), message, len);	

	return 0;
}
