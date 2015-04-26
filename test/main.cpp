#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <string.h>
#include <strings.h>

#include <message.pb.h>
using namespace message;

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("usage: %s ip port\n", basename(argv[0]));
		exit(EXIT_FAILURE);
	}

    signal(SIGPIPE, SIG_IGN);

	const char *ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd <= 0) {
		printf("Create Socket Failed\n");
		exit(EXIT_FAILURE);
	}
	
	if (connect(sockfd, (struct sockaddr*) &address, sizeof(address)) < 0) {
		printf("Create Socket Failed\n");
		exit(EXIT_FAILURE);
	}

	Message message;
	Header header;
	header.set_flow_no(1234);
	header.set_length(0);
	header.set_src_fsm(4321);
	header.set_dst_fsm(3123);
	message.set_allocated_header(&header);

	int size = message.ByteSize();
	printf("Message Size: %d \n", size);
	void *buffer = malloc(size);
	message.SerializeToArray(buffer, size);
	
	send(sockfd, buffer, size, 0);

	free(buffer);
	
	return 0;
}
