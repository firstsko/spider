#ifndef _FSM_CONTAINER_
#define _FSM_CONTAINER_

#include <map>

#include "sock.h"
#include "fsm.h"

class FsmContainer {
public:
	~FsmContainer();

	int AddStateMachine(unsigned machine_id, Fsm *pfsm);

	Fsm* GetStateMachine(unsigned machine_id);

	int ActivateCb(SMessage *pmessage, unsigned machine_id, int dst_status);
	
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
