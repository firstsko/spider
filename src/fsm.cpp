#include "fsm.h"
#include "fsm_container.h"
#include "fsm_factory.h"

static unsigned machine_flow_number = 0;

static unsigned generate_machine_number() {
	return ++machine_flow_number;
}

int Fsm::OnMessage(SMessage *pmessage) {
	int message_type = pmessage->header().type();
	int dst_state = pmessage->header().dst_state();
	int dst_fsm_id = pmessage->header().dst_state();

	// Initial State
	if (dst_state == 0)	{
		// Create StateMachine
	} else {
		Fsm* pstatemachine = FsmContainer::Instance()->GetStateMachine(dst_fsm_id);
		if (pstatemachine == NULL) {
			return FSM_NOTEXIST;
		} else {
			Status_t ret = pstatemachine->ActivateCb(pmessage, dst_state);
			return ret;
		}
	}
}

Fsm::Fsm() {
	machine_id_ = generate_machine_number();
	status_ = FSM_START;
	FsmContainer::Instance()->AddStateMachine(machine_id_, this);
}

Fsm::~Fsm() {
	FsmContainer::Instance()->DelStateMachine(machine_id_);
}
