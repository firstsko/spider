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

class Fsm;
typedef Status_t (*state_cb_t) (Fsm *, void *);

// Finite State Machine
class Fsm:public Channel {
public:
	Fsm();

	virtual ~Fsm();

	unsigned MyId() {
		return machine_id_;
	}

	static int OnMessage(SMessage *);
	
	virtual Status_t InvokeCb(SMessage *);

	virtual int FsmType() = 0;

    static int SetGlobalStateName(int type, int state, state_cb_t callback);

protected:
	// <machine_id, <state, callback>>
	static std::map<int, std::map<int, state_cb_t> > fsm_callbacks_;

private:
	int machine_id_;
	int state_;	
};

#define STATE_CALLBACK(type, state, classname, functionname)                        \
    static Status_t classname##functionname(Fsm *pfsm, void *pmessage) {    \
        return ((classname *)pfsm)->functionname(pmessage);                          \
    }                                                                                           \
    static int bUnused1##functionName __attribute__((unused)) =                                \
        Fsm::SetGlobalStateName(type, classname::state, classname##functionname);
#endif
