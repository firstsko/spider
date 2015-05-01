#ifndef _FSM_CONTAINER_
#define _FSM_CONTAINER_

#include <map>

#include "sock.h"
#include "fsm.h"

class FsmContainer {
public:
	~FsmContainer();

	int AddStateMachine(int message_type, Fsm *pfsm);

	int ActivateCb(SMessage *pmessage, int message_type, int dst_state);
	
	int DelStateMachine(int message_type);

	static FsmContainer* Instance();

private:
	// Private Constructor For Singletion
	FsmContainer() {};

private:
	std::map <int, Fsm *> fsm_container_;

	static FsmContainer* p_container_;
};

#endif
