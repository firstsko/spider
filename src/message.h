#ifndef _MESSAGE_
#define _MESSAGE_

#include  "message.pb.h"

typedef struct {
	unsigned length;
	unsigned message_id; 
} Header_t;

#endif
