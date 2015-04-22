#ifndef _SOCK_H_
#define _SOCK_H_

#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

#include "server.h"

// 20KB InBuffer And OutBuffer
#define SOCKET_BUFFER_SIZE (20*1024)

typedef enum {
	SOCK_IDLE,
	SOCK_LISTENNING,
	SOCK_CONNECTTING,
	SOCK_TCP_ENSTABLISHED,	
	SOCK_CLOSED,
	SOCK_TIMEOUT, // Unused Now
	SOCK_SHUT_ALL,
	SOCK_SHUT_READ,
	SOCK_SHUT_WRITE,
} Sockstate_t;

typedef enum {
	TYPE_TCP,
	TYPE_UDP,
	TYPE_RAW,
} Socktype_t;

// Only For TCP Stream And Only For Non-Blocking Operations
class Socket
{
 public:
	Socket(int fd); 
	
	~Socket();

	int GetFd() {
		return sockfd_;
	}

	Sockstate_t State() {
		return state_;
	}

	void Close() {
		close(sockfd_);
		state_ = SOCK_CLOSED;
	}

	int BindListen(int port);

	// Only Support Edge Trigger In Epoll Model
	int Read();

	// Only Support Edge Trigger In Epoll Model
	int Write();
	
	// Only For Non-Blocking Connect With A Timout 10 Second As Default
	int Connect(const std::string &ip, int port, int timeout = 10);

	static int Accept(int listen_fd);

	int SetTcpInBuffsize(size_t size);

	int SetTcpOutBuffsize(size_t size);

	void ShutdownAll();

	void ClearRBuffer() {
		memset(inbuf_, 0, SOCKET_BUFFER_SIZE * sizeof(char));
		r_offset_ = 0;
	}

	void ClearWBuffer() {
		memset(outbuf_, 0, SOCKET_BUFFER_SIZE * sizeof(char));
		w_offset_ = 0;
		append_offset_ = 0;
	}

	char *GetWriteIndex() {
		return outbuf_ + append_offset_;
	}

	// Add Data To Output Buffer, Wait To Be Sent
	void AppendSend(void *message, size_t len) {
		append_offset_ += len;
	}

	// Will Send All Remaining Data In TCP Sending Buffer And Then Send A TCP_FIN
	void ShutdownW();

	void ShutdownR();

	sockaddr_in GetPeerAddr() {
		return peer_;
	}
	
private:
	// Non-Blocking I/O
	void SetNonBlocking();

	// Turn Off Nagle's Algorithm And Delayed ACK
	void SetNoTcpDelay();

	// Address Reuse  
	void SetResueAddr();

	// Enable TCP Keep-Alive
	void SetKeepAlive();

	// Cannot Be Invoked By User
	void SetState(Sockstate_t state) {
		state_ = state;
	}

	void SetPeerAddr(sockaddr_in address) {
		peer_ = address;
	}
	
private:
	int sockfd_;
	sockaddr_in peer_;
	Sockstate_t state_;
	
	size_t r_offset_;
	size_t w_offset_;
	size_t append_offset_;
	char *inbuf_;
	char *outbuf_;
	Socktype_t type_;
};

#endif 
