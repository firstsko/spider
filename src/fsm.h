#ifndef _FINITE_STATE_MACHINE_
#define _FINITE_STATE_MACHINE_

#include <string>
#include <string.h>

#include "channel.h"
#include "server.h"
#include "sock.h"

typedef enum {
	FSM_START =  0,
	FSM_NEXT =  1,
	FSM_FINISH = 100,
	FSM_ERROR = -1,
	FSM_TIMEOUT = -2,
} State_t; 

// Finite State Machine
class Fsm: public Channel {
public:




private:
	State_t state_;
};

#endif
