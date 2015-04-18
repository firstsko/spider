#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "version.h"
#include "server.h"
#include "event_driver.h"
#include "timer.h"
#include "log.h"

using namespace std;

int bar(void* data) {
	write(1, now_str().c_str(), now_str().length());
	return 0;
}
	
int main(int argc , char **argv) {
	if (argc != 2) {
		printf("usage: %s port_number\n", basename(argv[0]));
		exit(EXIT_FAILURE);
	}

	Log *log = Log::Instance();
	signal(SIGPIPE, SIG_IGN);
	

	int port = atoi(argv[1]);

	int fd = socket(PF_INET, SOCK_STREAM, 0);

	Socket *server = new Socket(fd);

	server->BindListen(port);

	printf("%s Server Start, pid %d, Listen Fd %d On TCP Port %d\n", version, getpid(), server->GetFd(), port);

	EventDriver *driver = EventDriver::Instance();
	driver->CreateDriver();

	// Listen Fd Use Edge-Trigger
	driver->AddEvent(fd, server, EDGE_TRIGGER);

	driver->AddTimer(0, 500, false, bar);

	driver->StartLoop();

	delete server;
	delete driver;
	delete log;

	return 0;
}
