#ifndef _FINITE_STATE_MACHINE_
#define _FINITE_STATE_MACHINE_

#include <string>
#include <string.h>

#include "channel.h"
#include "server.h"
#include "sock.h"

typedef enum {
	FSM_TIMEOUT = -2,
	FSM_ERROR = -1,
	FSM_START = 0,
	FSM_NEXT = 1,
	FSM_FINISH = 100,
} State_t; 

// Finite State Machine
class Fsm: public Channel {
public:
	Fsm();

	~Fsm();

private:
	int Fsm_id_;
	State_t state_;
};

#endif
