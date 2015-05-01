#include "fsm.h"
#include "fsm_container.h"

int Fsm::OnMessage(SMessage *pmessage) {
	int message_type = pmessage->header().type();
	int dst_state = pmessage->header().dst_state();
	int dst_fsm_id = pmessage->header().dst_state();
	// Initial State
	if (dst_state == 0)	 {
		//
	} else {
		//
	}
	
	return 0;
}

Fsm::Fsm() {
	FsmContainer::Instance()->AddStateMachine(machine_id_, this);
}

Fsm::~Fsm() {
	FsmContainer::Instance()->DelStateMachine(machine_id_);
}
