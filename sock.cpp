#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h> // TCP_NODELAY
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#include "sock.h"
#include "event_driver.h"

#define TCP_BUFFER_SIZE 1024

using namespace std;

// Initialize File Descriptor, Set No Blocking , No Delay, Address Reuse, KeepAlive
Socket::Socket(int fd):sockfd_(fd), state_(SOCK_IDLE), inbuf_(""), outbuf_("") {
	SetNonBlocking();
	SetNoTcpDelay();
	SetResueAddr();
	SetKeepAlive();
}

Socket::~Socket() {
	ClearBuf();
	if (state_ != SOCK_CLOSED) {
		Close();		
	}
}

void Socket::SetNonBlocking() {
	if (fcntl(sockfd_, F_SETFL, O_NONBLOCK | fcntl(sockfd_, F_GETFL)) != 0) {
		PrintErrno();
	}
}

void Socket::SetNoTcpDelay() {
	int no_delay = 1;
	if (setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &no_delay, sizeof(no_delay)) != 0) {
		PrintErrno();
	}
}

void Socket::SetKeepAlive() {
	int keep_alive = 1;
	if (setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &keep_alive, sizeof(keep_alive)) != 0) {
		PrintErrno();
	}
}

void Socket::SetResueAddr() {
	int resue = 1;
	if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &resue, sizeof(resue)) != 0) {
		PrintErrno();
	}
}

int Socket::Connect(const string &ip, int port, int second) {
	int ret = 0;
	struct sockaddr_in address;	
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;

	inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
	address.sin_port = htons(port);

	state_ = SOCK_CONNECTTING,
	ret = connect(sockfd_, (struct sockaddr *) &address, sizeof(address));
	
	// If Succeed Immediately
	if (ret == 0) {
		return ret;
	} else if (errno != EINPROGRESS)  { // Connect Fail
		PrintErrno();
		return -1;	
	}	
	
	fd_set rfds;
	fd_set wfds;

	struct timeval timeout;
	timeout.tv_sec = second;
	timeout.tv_usec = 0;
	
	FD_ZERO(&rfds);
	FD_SET(sockfd_, &wfds);

	// Our Connection Is Completed When Socket Becomes Writable Without Errors
	ret = select(sockfd_ + 1, NULL, &wfds, NULL, &timeout);
	if (ret <= 0) {
		PrintErrno();
		return - 1;
	}

	if (!FD_ISSET(sockfd_, &wfds)) {
		return -1;
	}
	
	int error = 0;
	socklen_t length = sizeof(error);

	ret = getsockopt(sockfd_, SOL_SOCKET, SO_ERROR, &error, &length);
	if (ret < 0) {
		return -1;	
	}

	if (error != 0)	{
		return - 1;
	}

	state_ = SOCK_TCP_ENSTABLISHED;
	return 0;
}

// NonBlocking And Edge-Trigger
int Socket::Accept(int listen_fd) {
	int peer_fd = 0;
	sockaddr_in peer_addr;
	socklen_t addr_len = sizeof(peer_addr);
	
	while(true) {
		peer_fd = accept(listen_fd, (sockaddr *)&peer_addr, &addr_len);
		if (peer_fd == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			} else if (errno == EINTR) {
				continue;
			} else {
				PrintErrno();
				return -1;
			}
		} else {
			Socket* peer = new Socket(peer_fd);
			peer->SetState(SOCK_TCP_ENSTABLISHED);
			peer->SetPeerAddr(peer_addr);
			EventDriver::Instance()->AddEvent(peer_fd, peer);
		}
	}

	return 0;
}

int Socket::BindListen(int port) {
	int ret = 0;

	struct sockaddr_in address;	
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	
	ret = bind(sockfd_, (struct sockaddr *) &address, sizeof(address));
	if (ret == -1) {
		PrintErrno();
		return ret;
	}
	
	ret = listen(sockfd_, 5);
	if (ret == -1) {
		PrintErrno();
		return ret;
	}

	state_ = SOCK_LISTENNING;
	return ret;
}

int Socket::Read() {
	int len = 0;		
	int bytes = 0;
	char buf[TCP_BUFFER_SIZE] = {0};

	while (true) {
		memset(buf, 0, TCP_BUFFER_SIZE);
		len = recv(sockfd_, buf, TCP_BUFFER_SIZE - 1, 0);
		bytes += len;
		if (len < 0)  {
			// All Data Drained For Edge-Trigger, No More Data, Wait For Next Round
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			} else if (errno == ECONNRESET) { // Recieve A TCP_RST From Peer 
				PrintErrno();
				Close();
				state_ = SOCK_CLOSED;
				break;
			} else if (errno == EINTR) {	// Interupted By A Signal
				continue;
			} 	
			// Other Errors, Close Connection
			Close(); 
		} else if (len == 0) {
			// TCP_FIN Recieved From Peer, Close Connection
			Close(); 
			state_ = SOCK_CLOSED;
			break;
		} else {
			inbuf_.append(buf);
		}
	}

	// User Can Check Return Value To Determine Where Connection Is OK
	return bytes;
}

int Socket::Write() {
	// No Data To Send
	if (outbuf_.empty()) {
		printf("No Data To Send\n");
		return 0;
	}

	int sendsize = outbuf_.size();	
	printf("Send Size %d\n", sendsize);
	int len = 0;		
	int bytes = 0;

	while (true) {
		len = send(sockfd_, outbuf_.c_str() + bytes, sendsize, 0);
		if (len < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// All Data Sent In Non-Blocking Socket
				break;
			} else if (errno == ECONNRESET) { // Recieve A TCP_RST From Peer 
				PrintErrno();
				Close();
				state_ = SOCK_CLOSED;
				break;
			} else if (errno == EINTR) { // Interupted By A Signal
				continue;
			}
		} else {
			bytes += len;

			// All Data Sent
			if (bytes >= sendsize) {
				break;	
			}
		}
	}
	return bytes;
}

void Socket::ShutdownAll() {
	shutdown(sockfd_, SHUT_RDWR);
	state_ = SOCK_SHUT_ALL;
}

void Socket::ShutdownR() {
	shutdown(sockfd_, SHUT_RD);
	state_ = SOCK_SHUT_READ;
}

void Socket::ShutdownW() {
	shutdown(sockfd_, SHUT_WR);
	state_ = SOCK_SHUT_WRITE;
}

int Socket::SetTcpInBuffsize(size_t size) {
	int ret = setsockopt(sockfd_, SOL_SOCKET, SO_RCVBUF, &size, 4);
	if (ret < 0) {
		PrintErrno();
	}
	return ret;
}

int Socket::SetTcpOutBuffsize(size_t size) {
	int ret = setsockopt(sockfd_, SOL_SOCKET, SO_SNDBUF, &size, 4);
	if (ret < 0) {
		PrintErrno();
	}
	return ret;
}
