#include "fsm.h"

int Fsm::OnMessage(SMessage *pmessage) {
	int message_type = psmessage->body().type();
	int dst_state = psmessage->header().dst_state();
	// Initial State
	if (dst_state == 0)	 {
		return FsmContainer::Instance()->CreateStateMachine(message_type);
	} else {
		return FsmContainer::Instance()->ActivateCb(*pessage, message_type, dst_state);
	}
}

Fsm::Fsm(int id) {
	Fsm_id_ = id;
	FsmContainer::Instance->AddStateMachine(Fsm_id_);
}

Fsm::~Fsm() {
	FsmContainer::Instance->DelStateMachine(Fsm_id_);
}
