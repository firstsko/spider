#include "fsm.h"

int Fsm::OnMessage(SMessage *pmessage) {
	int message_type = psmessage->body().type();
	int dst_state = psmessage->header().dst_state();
	int dst_fsm_id = psmessage->header().dst_state();
	// Initial State
	if (dst_state == 0)	 {
		return FsmContainer::Instance()->CreateStateMachine(message_type);
	} else {
		return FsmContainer::Instance()->ActivateCb(*pessage, message_type, dst_state);
	}
}

Fsm::Fsm() {
	FsmContainer::Instance->AddStateMachine(machine_id_);
}

Fsm::~Fsm() {
	FsmContainer::Instance->DelStateMachine(machine_id_);
}
