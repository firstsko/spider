#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h> // TCP_NODELAY
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <stdlib.h>
#include <map>

#include "sock.h"
#include "event_driver.h"
#include "message.h"

using namespace std;
using namespace spider;

// Collect Current TCP Connections
map<sockaddr_in, Socket *> gmap_tcpdest;

static char* iptostr(unsigned ip) {
	struct in_addr addr;
	memcpy(&addr, &ip, 4);
	return inet_ntoa(addr);
}

// Initialize File Descriptor, Set No Blocking, No Delay, Address Reuse, KeepAlive
Socket::Socket(int fd):sockfd_(fd), state_(SOCK_IDLE), r_offset_(0), w_offset_(0), append_offset_(0), inbuf_(NULL), outbuf_(NULL), 
	type_(TYPE_TCP)
 {
	inbuf_ = (char *)malloc(SOCKET_BUFFER_SIZE * sizeof(char));
	if (inbuf_ == NULL) {
		ALERT("Cannot Allocate In Buffer");
	}
	outbuf_ = (char *)malloc(SOCKET_BUFFER_SIZE * sizeof(char));
	if (inbuf_ == NULL) {
		ALERT("Cannot Allocate Out Buffer");
	}
	ClearRBuffer();
	ClearWBuffer();

	SetNonBlocking();
	SetNoTcpDelay();
	SetResueAddr();
	SetKeepAlive();

	recv_buff_size_ = GetTcpInBuffsize();
	send_buff_size_ = GetTcpOutBuffsize();
	last_io_time_ = time(NULL);
}

void Socket::Close() {
	// Avoiding Double Close In Destructor
	if (state_ != SOCK_CLOSED) {
		close(sockfd_);
		state_ = SOCK_CLOSED;
		INFO("Close Connection With Client %s:%d", iptostr(peer_.sin_addr.s_addr), ntohs(peer_.sin_port));
	}
}

Socket::~Socket() {
	if (inbuf_ != NULL) {
		free(inbuf_);
		inbuf_ = NULL;
	}

	if (outbuf_ != NULL) {
		free(outbuf_);
		outbuf_ = NULL;
	}

	if (state_ != SOCK_CLOSED) {
		Close();		
	}
}

void Socket::SetNonBlocking() {
	if (fcntl(sockfd_, F_SETFL, O_NONBLOCK | fcntl(sockfd_, F_GETFL)) != 0) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
	}
}

void Socket::SetNoTcpDelay() {
	int no_delay = 1;
	if (setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &no_delay, sizeof(no_delay)) != 0) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
	}
}

void Socket::SetKeepAlive() {
	int keep_alive = 1;
	if (setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &keep_alive, sizeof(keep_alive)) != 0) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
	}
}

void Socket::SetResueAddr() {
	int resue = 1;
	if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &resue, sizeof(resue)) != 0) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
	}
}

int Socket::Connect(const string &ip, int port, int second) {
	int ret = 0;
	struct sockaddr_in address;	
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;

	inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
	address.sin_port = htons(port);
	SetPeerAddr(address);

	state_ = SOCK_CONNECTTING,
	ret = connect(sockfd_, (struct sockaddr *) &address, sizeof(address));

	// If Succeed Immediately
	if (ret == 0) {
		state_ = SOCK_TCP_ENSTABLISHED;
		EventDriver::Instance()->AddEvent(sockfd_, this);
		return ret;
	} else if (errno != EINPROGRESS)  { // Connect Fail
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
		return -1;	
	} else {
		// Connection In Progress
		EventDriver::Instance()->AddEvent(sockfd_, this);
		return SOCK_CONNECTTING;
	}
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
				ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
				return -1;
			}
		} else {
			INFO("Accept Connection From Client %s:%d", iptostr(peer_addr.sin_addr.s_addr), ntohs(peer_addr.sin_port));
			Socket* peer = new Socket(peer_fd);
			peer->SetState(SOCK_TCP_ENSTABLISHED);
			peer->SetPeerAddr(peer_addr);
			EventDriver::Instance()->AddEvent(peer_fd, peer);
			// Record Socket's Peer Address
			gmap_tcpdest.insert(make_pair(peer_addr, peer));
		}
	}

	return 0;
}

// If This Function Fails, Process Will Exit Immediately
int Socket::BindListen(int port) {
	int ret = 0;

	struct sockaddr_in address;	
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	
	ret = bind(sockfd_, (struct sockaddr *) &address, sizeof(address));
	if (ret == -1) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
		printf("Fatal Error: %s. Program Abort!\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	ret = listen(sockfd_, 5);
	if (ret == -1) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
		printf("Fatal Error: %s. Program Abort!\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	state_ = SOCK_LISTENNING;
	return ret;
}

int Socket::Read() {
	UpdateTimeStamp();
	int len = 0;		
	int bytes = 0;

	while (true) {
		len = recv(sockfd_, inbuf_ + r_offset_, SOCKET_BUFFER_SIZE - 1, 0);
		bytes += len;
		r_offset_ += len;

		if (len < 0)  {
			// All Data Drained For Edge-Trigger, No More Data, Wait For Next Round
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			} else if (errno == ECONNRESET) { // Recieve A TCP_RST From Peer 
				ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
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
			break;
		} 
	}

	if (bytes > (int)sizeof(Header_t)) {
		Header_t *head;
		head = (Header_t *)inbuf_;
		int msg_len = ntohl(head->length);
		if (msg_len > SOCKET_BUFFER_SIZE) {
			INFO("Socket Inbuffer Not Enough, Allocate Space Again");
			free(inbuf_);
			inbuf_ = (char *)malloc((msg_len + 1) * sizeof(char));
			if (inbuf_ == NULL) {
				EMERG("Cannot Allocate In Buffer, Process Will Exit");
				exit(EXIT_FAILURE);
			}
		}
	
		// int msg_id = ntohl(head->message_id);
		if (bytes < msg_len) {
			return bytes;
		} else if (bytes == msg_len) {
		//	Message *msg_ptr = (Message *) inbuf_; 
		//	DispathMesasage(*msg_ptr);
			ClearRBuffer();	
		} else {
		//	Message *msg_ptr = (Message *) inbuf_; 
		//	DispathMesasage(*msg_ptr);
			memcpy(inbuf_, inbuf_ + msg_len, bytes - msg_len);
			memset(inbuf_ + bytes - msg_len, 0, msg_len);
			r_offset_ = bytes - msg_len;			
		} 

		if (msg_len > SOCKET_BUFFER_SIZE) {
			free(inbuf_);
			inbuf_ = (char *)malloc(SOCKET_BUFFER_SIZE * sizeof(char));
			if (inbuf_ == NULL) {
				EMERG("Cannot Allocate In Buffer, Process Will Exit");
				exit(EXIT_FAILURE);
			}
		}
	}	

	// User Can Check Return Value To Determine Where Connection Is OK
	return bytes;
}

int Socket::Write() {
	UpdateTimeStamp();
	// On Connection Complete And Socket Become Writable
	if (state_ == SOCK_CONNECTTING) {
		int ret = 0;
		int error = 0;
		socklen_t length = sizeof(error);

		ret = getsockopt(sockfd_, SOL_SOCKET, SO_ERROR, &error, &length);
		if (ret < 0) {
			return -1;	
		}

		if (error != 0)	{
			return - 1;
		}

		INFO("Connection With Server %s:%d Enstablished", iptostr(peer_.sin_addr.s_addr), peer_.sin_port);
		state_ = SOCK_TCP_ENSTABLISHED;
		gmap_tcpdest.insert(make_pair(peer_, this));
	}

	// No Data To Send
	if (append_offset_ == 0) {
		INFO("No Data To Send");
		return 0;
	}

	int len = 0;		
	int bytes = 0;

	while (true) {
		len = send(sockfd_, outbuf_ + w_offset_, SOCKET_BUFFER_SIZE, 0);
		if (len < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// All Data Sent In Non-Blocking Socket
				break;
			} else if (errno == ECONNRESET) { // Recieve A TCP_RST From Peer 
				ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
				Close();
				state_ = SOCK_CLOSED;
				break;
			} else if (errno == EINTR) { // Interupted By A Signal
				continue;
			}
		} else {
			bytes += len;
			w_offset_ += len;
			append_offset_ -= len;
			// All Date Sent
			if (append_offset_ == 0) {
				ClearWBuffer();
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
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
	}
	recv_buff_size_ = size;
	return ret;
}

int Socket::SetTcpOutBuffsize(size_t size) {
	int ret = setsockopt(sockfd_, SOL_SOCKET, SO_SNDBUF, &size, 4);
	if (ret < 0) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
	}
	send_buff_size_ = size;
	return ret;
}

int Socket::GetTcpInBuffsize() {
	int size;
	int	len = sizeof(size);
	int ret = getsockopt(sockfd_, SOL_SOCKET, SO_RCVBUF, &size, (socklen_t*) &len);
	if (ret < 0) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
	}
	return size;
}

int Socket::GetTcpOutBuffsize() {
	int size;
	int	len = sizeof(size);
	int ret = getsockopt(sockfd_, SOL_SOCKET, SO_SNDBUF, &size, (socklen_t*) &len);
	if (ret < 0) {
		ERROR("Errno: %d, ErrStr: %s", errno, strerror(errno));
	}
	return size;
}
