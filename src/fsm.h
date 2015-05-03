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


// Finite State Machine
class Fsm:public Channel {
public:
	typedef Status_t (Fsm::*state_cb_t) (void *);
	Fsm();

	~Fsm();

	unsigned MyId() {
		return machine_id_;
	}

	static int OnMessage(SMessage *);
	
	int SetCallback(int state, state_cb_t callback) {
		std::map <int, state_cb_t> tmp;
		tmp.insert(std::make_pair(state, callback));
		fsm_callbacks_.insert(std::make_pair(machine_id_, tmp));
		return 0;
	}
	
	virtual Status_t InvokeCb(SMessage *, int state);

protected:
	// <machine_id, <state, callback>>
	static std::map<int, std::map<int, state_cb_t> > fsm_callbacks_;

private:
	int machine_id_;
	int state;	
};

#endif
