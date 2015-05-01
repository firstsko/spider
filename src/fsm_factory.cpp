#include "fsm_factory.h"

using namespace std;

FsmFactory* FsmFactory::pfactory_ = NULL;

FsmFactory::~FsmFactory() {
}

// Singleton, Not Multi-Thread Safe
FsmFactory* FsmFactory::Instance() {
	if (pfactory_ == NULL) {
		pfactory_ = new FsmFactory();
	}

	return pfactory_;
}

int FsmFactory::AddStateMachine(int request_type, FsmCreate_t create_cb) {
	fsm_factory_.insert(make_pair(request_type, create_cb));
	return 0;
}

int FsmFactory::DelStateMachine(int request_type) {
	fsm_factory_.erase(request_type);
	return 0;
}
