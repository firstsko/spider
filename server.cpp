#include <sys/socket.h>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>

#include "server.h"
#include "event_driver.h"
#include "timer.h"
#include "log.h"

static const char *version = "Spider0.0.2";

using namespace std;

int bar(void* data) {
	INFO("Now %s", now_str().c_str());
	return 0;
}

int flush_log(void* data) {
	Log::Instance()->Flush();
	return 0;
}

void dump_stacktrace(int signal) {
	printf("Segmentation Fault!\n");
	CRIT("Segmentation Fault!");
	// Flush Log
	Log::Instance()->Flush();

	void *stack[64];
	size_t size;
	char **symbols;
	size = backtrace(stack, 64);
	symbols = backtrace_symbols(stack, size);

	if (symbols == NULL) {
		CRIT("backtrace_symbols");
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	printf("Obtained %lu Stack Frames\n", size);
	CRIT("Obtained %lu Stack Frames", size);
	for (size_t i = 0; i < size; i++) {
		CRIT("%s", symbols[i]);
		printf("%s\n", symbols[i]);
	}

	free(symbols);
	CRIT("Process Abort!");
	Log::Instance()->Flush();
	printf("Process Abort!\n");
	exit(EXIT_FAILURE);
}

int main(int argc , char **argv) {
	if (argc != 2) {
		printf("usage: %s port_number\n", basename(argv[0]));
		exit(EXIT_FAILURE);
	}

	// When Segmentation Fault Occurs, Print Diagnose Information
	signal(SIGSEGV, dump_stacktrace);
	signal(SIGPIPE, SIG_IGN);

	int port = atoi(argv[1]);
	int fd = socket(PF_INET, SOCK_STREAM, 0);

	Socket *server = new Socket(fd);

	server->BindListen(port);

	INFO("%s Server Start, pid %d, Listen Fd %d On TCP Port %d", version, getpid(), server->GetFd(), port);
	printf("%s %s Server Start, pid %d, Listen Fd %d On TCP Port %d\n", now_str().c_str(), version, getpid(), server->GetFd(), port);

	EventDriver *driver = EventDriver::Instance();
	driver->CreateDriver();

	// Listen Fd Use Edge-Trigger
	driver->AddEvent(fd, server, EDGE_TRIGGER);

	driver->AddTimer(0, 500, false, bar);
	// Every 100ms Flush Log Cache Buffer
	driver->AddTimer(0, 100, false, flush_log);

	driver->StartLoop();

	delete server;
	delete driver;

	return 0;
}
