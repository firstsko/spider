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
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

#include "message.pb.h"

using namespace spider;

typedef struct {
	unsigned length;
	unsigned message_id; 
} header;

google::protobuf::Message* CreateMessage(const std::string &name) {
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor =
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(name);
	if (descriptor) {
		const google::protobuf::Message* prototype =
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
			message = prototype->New();
		}
	}

	return message;
}

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

	SMessage *message = new SMessage;
	Header *head = new Header;
	head->set_flow_no(1234);
	head->set_src_fsm(4321);
	head->set_dst_fsm(3123);
	head->set_type(HEART_BEAT_REQUEST);
	message->set_allocated_header(head);

	int size = message->ByteSize();
	int length = size + sizeof(header);
	printf("Message Size: %d Header Size:%d \n", length, length - size);
	void *buffer = malloc(length);
	header h;
	h.length = htonl(length);
	h.message_id = htonl(LOGIN_REQUEST);
	memcpy(buffer, &h, sizeof(h));
	message->SerializeToArray((char *)buffer + sizeof(header), size);
	
	printf("Message: %s\n", message->DebugString().c_str());	
	printf("Send %lu Bytes\n", send(sockfd, buffer, length, 0));
	
	delete message;
	free(buffer);

	return 0;
}
