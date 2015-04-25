// g++ -Wall -g -o test unit_test.cc
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <execinfo.h>
//include "foo.h"

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

int hello() {
	char b[3] = {'a', 'b', 'c'};
	int *a = (int *) 0x00000001;
	printf("%d%c\n", *a, b[100]);

	return 0;
}

int main () {
	signal(SIGSEGV, print_trace);
	hello();
	return 0;
}
