#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "event_driver.h"

using namespace std;
	
int main(int argc , char **argv) {
	if (argc != 2) {
		printf("usage: %s port_number\n", basename(argv[0]));
		exit(EXIT_FAILURE);
	}

	signal(SIGPIPE, SIG_IGN);

	int port = atoi(argv[1]);

	int fd = socket(PF_INET, SOCK_STREAM, 0);

	Socket *server = new Socket(fd);

	server->BindListen(port);

	printf("Login Server Start, pid %d, Listen Fd %d\n", getpid(), server->GetFd());

	EventDriver *driver = EventDriver::Instance();
	driver->CreateDriver();

	// Listen Fd Use Level-Trigger
	driver->AddEvent(fd, server, LEVEL_TRIGGER);

	driver->StartLoop();

	delete server;
	delete driver;

	return 0;
}
