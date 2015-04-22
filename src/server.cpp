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

void flush_log(void) {
	Log::Instance()->Flush();
}

void ctrl_c_handler(int signal) {
	EMERG("Process Interupted By Signal(%d) SIGINT(CTRL+C)", signal);
	printf("Process Interupted By SIGINT(CTRL+C)\n");
	Log::Instance()->Flush();
	exit(EXIT_FAILURE);
}

void dump_stacktrace(int signal) {
	printf("Signal(%d) Segmentation Fault!\n", signal);
	CRIT("Signal(%d) Segmentation Fault!", signal);
	// Flush Log
	Log::Instance()->Flush();

	void *stack[64];
	size_t size;
	char **symbols;
	size = backtrace(stack, 64);
	symbols = backtrace_symbols(stack, size);

	if (symbols == NULL) {
		CRIT("No Backtrace Symbols");
		perror("No Backtrace Symbols");
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
	signal(SIGINT, ctrl_c_handler);

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
	// Every 100ms Flush Log Cache Buffer
	driver->AddTimer(0, 100, false, flush_log, NULL);

	driver->AddTimer(0, 500, false, bar, NULL);

	driver->StartLoop();

	delete server;
	delete driver;

	atexit(flush_log);
	return 0;
}
