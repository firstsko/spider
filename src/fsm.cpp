#include "fsm.h"
#include "fsm_container.h"
#include "fsm_factory.h"

using namespace std;

map<int, map<int, state_cb_t> > Fsm::fsm_callbacks_;

static int machine_flow_number = 0;

static int generate_machine_number() {
	return ++machine_flow_number;
}

int Fsm::OnMessage(SMessage *pmessage) {
	int message_type = pmessage->header().type();
	int dst_state = pmessage->header().dst_state();
	int dst_fsm_id = pmessage->header().dst_state();

	// Initial State
	if (dst_state == 0 || dst_fsm_id == 0)	{

		// Create StateMachine
		return 0;
	} else {
		Fsm* pstatemachine = FsmContainer::Instance()->GetStateMachine(dst_fsm_id);
		if (pstatemachine == NULL) {
			pstatemachine = FsmContainer::Instance()->NewStateMachine(message_type);
			if (pstatemachine == NULL) {
				ERROR("Cannot Find StateMachine To Handle Incoming Message");
				return FSM_NOTEXIST;
			}
			Status_t ret = pstatemachine->InvokeCb(pmessage, dst_state);
			if (ret == FSM_ERROR || ret == FSM_FINISH) {
				delete pstatemachine;
			}
			return ret;
		} else {
			Status_t ret = pstatemachine->InvokeCb(pmessage, dst_state);
			if (ret == FSM_ERROR || ret == FSM_FINISH) {
				delete pstatemachine;
			}
			return ret;
		}
	}
}

int Fsm::SetGlobalStateName(int type, int state, state_cb_t callback) {
	map<int, state_cb_t> tmp;
	tmp.insert(make_pair(state, callback));
	fsm_callbacks_.insert(make_pair(type, tmp));
	return 0;
}

Status_t Fsm::InvokeCb(SMessage *pmessage, int state) {

	return FSM_NEXT;
}

Fsm::Fsm() {
	machine_id_ = generate_machine_number();
	FsmContainer::Instance()->AddStateMachine(machine_id_, this);
}

Fsm::~Fsm() {
	FsmContainer::Instance()->DelStateMachine(machine_id_);
}
