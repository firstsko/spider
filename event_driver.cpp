#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include "event_driver.h"

using namespace std;

EventDriver* EventDriver::p_event_driver_ = NULL;

EventDriver::~EventDriver() {
	close(epfd_);
}

// Singleton, Not Multi-Thread Safe
EventDriver* EventDriver::Instance() {
	if (p_event_driver_ == NULL) {
		p_event_driver_ = new EventDriver();
	}
	return p_event_driver_;
}

void EventDriver::CreateDriver() {
	// Create Kernel Event Table
	epfd_ = epoll_create(5);
}

// Default Edge-Trigger, Level-Trigger Only For Listening Fd
void EventDriver::AddEvent(int fd, Socket *sk,  Trigger_t type) {
	epoll_event event;
	event.data.fd = fd;
	if (type == LEVEL_TRIGGER) {
		event.events = EPOLLIN | EPOLLRDHUP; 
	} else {
		event.events = EPOLLIN | EPOLLET | EPOLLRDHUP; 
	}
	epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event);
	event_container_.insert(make_pair(fd, sk));	
}

// Under Edge-Trigger  
void EventDriver::ModifyEvent(int fd, int active_type) {
	epoll_event event;
	event.data.fd = fd;
	if (active_type == EPOLLIN) {
		event.events = EPOLLIN | EPOLLET | EPOLLRDHUP; 
	} else if (active_type == EPOLLOUT) {
		event.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLRDHUP; 
	}
	epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &event);
}

void EventDriver::DelEvent(int fd) {
	epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL);
	delete event_container_[fd];
	event_container_.erase(fd);
}

void EventDriver::StartLoop(int timeout_usec) {
	epoll_event events[MAX_EVENT_NO];

	while (true) {
		int number = epoll_wait(epfd_, events, MAX_EVENT_NO, timeout_usec); 
		if (number < 0 ) {
			if (errno == EINTR) {
				continue;
			}
			this->PrintErrno();
			break;	
		}

		map<int, Socket *>::iterator it;

		for (int i = 0; i < number; i++) {
			int sockfd = events[i].data.fd;
		
			// Connection Closed By Peer
			if (events[i].events & EPOLLRDHUP) {

				if ((it = event_container_.find(sockfd)) != event_container_.end()) {
					it->second->Close();
					DelEvent(sockfd);
				}
			}
			// Recieve New Data
			if (events[i].events & EPOLLIN) {
				// Accept A New Connection, Level-Trigger
				if ((it = event_container_.find(sockfd)) != event_container_.end()) {
					if (it->second->State() == SOCK_LISTENNING) {
						// Note That New Socket Object Will Be Created
						int connfd = Socket::Accept(sockfd);
						if (connfd < 0) {
							PrintErrno();
						}
						continue;
					}
				}
				// Drain All Data From TCP Recv Buffer, If TCP_FIN Recieved, Remove The Event
				if ((it = event_container_.find(sockfd)) != event_container_.end()) {
					it->second->Read();
					// Remove Closed Socket
					if (it->second->State() == SOCK_CLOSED) {
						DelEvent(sockfd);
					} else {
						ModifyEvent(sockfd, EPOLLOUT);
					}
				}

			} 

			if (events[i].events & EPOLLOUT) { // Ready To Send Data
				// Send All Data
				if ((it = event_container_.find(sockfd)) != event_container_.end()) {
					it->second->Write();
					// Remove Closed Socket
					if (it->second->State() == SOCK_CLOSED) {
						DelEvent(sockfd);
					} else {
						ModifyEvent(sockfd, EPOLLIN);
					}
				}

			}	
		}
	}
	return;
}
