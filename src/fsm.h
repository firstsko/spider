#ifndef _FINITE_STATE_MACHINE_
#define _FINITE_STATE_MACHINE_

#include <map>

#include "channel.h"
#include "server.h"
#include "sock.h"
#include "message.h"

typedef enum {
	FSM_TIMEOUT = -2,
	FSM_ERROR = -1,
	FSM_START = 0,
	FSM_NEXT = 1,
	FSM_FINISH = 100,
} Status_t; 

typedef Status_t (*state_cb_t) (void *);

// Finite State Machine
class Fsm:public Channel {
public:
	Fsm();

	~Fsm();

	static int OnMessage(SMessage *);

private:
	unsigned machine_id_;
	Status_t status_;
	
	std::map<int, state_cb_t> fsm_callbacks_; 
};

#endif
