#include "fsm_container.h"

using namespace std;

FsmContainer* FsmContainer::p_container_ = NULL;

FsmContainer::~FsmContainer() {
}

// Singleton, Not Multi-Thread Safe
FsmContainer* FsmContainer::Instance() {
	if (p_container_ == NULL) {
		p_container_ = new FsmContainer();
	}
	return p_container_;
}

int FsmContainer::AddStateMachine(unsigned machine_id, Fsm *pfsm) {
	fsm_container_.insert(make_pair(machine_id, pfsm));
	return 0;
}

Fsm* GetStateMachine(unsigned machine_id) {
	if (fsm_container_.find(machine_id) == fsm_container_.end()) {
		ERROR("Cannot Find A State Machine To Process Incoming Message");
		return NULL;
	} else {
		return fsm_container_.[machine_id];

	}
}

int FsmContainer::ActivateCb(SMessage *pmessage, unsigned machine_id, int dst_state) {

	return 0;
}

int FsmContainer::DelStateMachine(int message_type) {
	fsm_container_.erase(message_type);
	return 0;
}
