#ifndef _MESSAGE_
#define _MESSAGE_

#include "message.pb.h"

using namespace message;

typedef struct {
	unsigned length;
	unsigned message_id; 
} Header_t;

class MessageUtil {	
public:
	static int OnMessage(void *pmsg, int msg_id);

private:
};

#endif
