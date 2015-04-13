#ifndef _SOCK_H_
#define _SOCK_H_

#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>

#include <string>
#include <string.h>
#include <errno.h>

typedef enum {
	SOCK_IDLE,
	SOCK_LISTENNING,
	SOCK_CONNECTTING,
	SOCK_TCP_ENSTABLISHED,	
	SOCK_CLOSED,
	SOCK_SHUT_ALL,
	SOCK_SHUT_READ,
	SOCK_SHUT_WRITE,
} Sockstate_t;

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
	
	// Only For Non-Blocking Connect With A Timout
	int Connect(const std::string &ip, int port, int timeout);

	static int Accept(int listen_fd);

	int SetTcpInBuffsize(size_t size);

	int SetTcpOutBuffsize(size_t size);

	void ClearBuf() {
		inbuf_.clear();
		outbuf_.clear();
	}

	void ShutdownAll();

	// Will Send All Remaining Data In TCP Sending Buffer And Then Send A TCP_FIN
	void ShutdownW();

	void ShutdownR();
	
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
	
	static void PrintErrno() {
		printf("%s %d Errno %d Info:%s\n", __FILE__, __LINE__, errno, strerror(errno));
	}

private:
	int sockfd_;
	sockaddr_in peer_;
	Sockstate_t state_;

	std::string inbuf_; 
	std::string outbuf_; 
};

#endif 
