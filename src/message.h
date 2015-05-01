#ifndef _MESSAGE_
#define _MESSAGE_

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include "message.pb.h"

using namespace spider;

typedef struct {
	unsigned length;
	unsigned message_id; 
} Header_t;

google::protobuf::Message* CreateMessage(const std::string &name);

#endif
