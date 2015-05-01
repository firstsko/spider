#include "fsm_factory.h"

using namespace std;

FsmFactory* FsmFactory::p_container_ = NULL;

FsmFactory::~FsmFactory() {
}

// Singleton, Not Multi-Thread Safe
FsmFactory* FsmFactory::Instance() {
	if (p_container_ == NULL) {
		p_container_ = new FsmFactory();
	}
	return p_event_driver_;
}

int FsmFactory::AddStateMachine(int message_type, Fsm *pfsm) {
	fsm_container_.insert(make_pair(message_type), pfsm);
	return 0;
}

int FsmFactory::ActivateCb(SMessage *pmessage, int message_type, int dst_state) {

	return 0;
}

int FsmFactory::DelStateMachine(int message_type) {
	fsm_container_.erase(message_type);
	return 0;
}
