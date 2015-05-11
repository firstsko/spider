// g++ -Wall -g -o test unit_test.cc
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <execinfo.h>
//include "foo.h"

using namespace std;

void print_trace(int signal) {
	// Flush Log
//	Log::Instance()->Flush();

	void *stack[10];
	size_t size;
	char **symbols;
	size = backtrace(stack, 10);
	symbols = backtrace_symbols(stack, size);

	if (symbols == NULL) {
//		ERROR("backtrace_symbols");
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < size; i++) {
//		CRIT("%s", symbols[i]);
		printf("%s\n", symbols[i]);
	}
	free(symbols);
	exit(EXIT_FAILURE);
}
/*
int hello() {
	char b[3] = {'a', 'b', 'c'};
	int *a = (int *) 0x00000001;
	printf("%d%c\n", *a, b[100]);

	return 0;
}*/

int main () {
	//signal(SIGSEGV, print_trace);
//	hello();
	const string proto_regex = "^(https?:\\/\\/)?";
	const string hostname_regex = "([0-9a-zA-Z-]+\\.)+[0-9a-zA-Z]{2,6}";
	const string port_regex = "(:[0-9]{1,5})?";
	const string path_regex = "(\\/([\\w]+\\/)*([\\w]\\.?[\\w])+)?";
	const string querystring_regex = "(\\?((([\\w%]+=[\\w%]+)&)*([\\w%]+=[\\w%]+)))?$";
	
	char buf[1024] = {0};
	snprintf(buf, sizeof(buf) - 1, "%s%s%s%s%s", proto_regex.c_str(), hostname_regex.c_str(), port_regex.c_str(), path_regex.c_str(), querystring_regex.c_str());
	string valid_http_url = buf;

	printf("regex: %s\n", valid_http_url.c_str());
	return 0;
}
