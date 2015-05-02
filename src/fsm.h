#ifndef _FINITE_STATE_MACHINE_
#define _FINITE_STATE_MACHINE_

#include <map>

#include "channel.h"
#include "server.h"
#include "sock.h"
#include "message.h"

typedef enum {
	FSM_NOTEXIST = -3,
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
	
	static int SetCallback(int type, int state, state_cb_t) {
		fsm_callbacks_.insert(make_pair(type, make_pair(state, state_cb_t)));
	}
	
	Status_t ActivateCb(SMessage *, int state);

	static int EraseCallback(int type) {
		fsm_callbacks_.erase(type);
	}

private:
	unsigned machine_id_;
	int State;	
	
	static std::map<int, std::map<int, state_cb_t>> fsm_callbacks_; 
};

#endif
