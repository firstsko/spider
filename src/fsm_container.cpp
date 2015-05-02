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

int FsmContainer::AddStateMachine(int machine_id, Fsm *pfsm) {
	fsm_container_.insert(make_pair(machine_id, pfsm));
	return 0;
}

Fsm* FsmContainer::GetStateMachine(int machine_id) {
	if (fsm_container_.find(machine_id) == fsm_container_.end()) {
		ERROR("Cannot Find A State Machine To Process Incoming Message");
		return NULL;
	} else {
		return fsm_container_[machine_id];
	}
}

int FsmContainer::DelStateMachine(int machine_id) {
	fsm_container_.erase(machine_id);
	return 0;
}
