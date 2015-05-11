#include "fsm_factory.h"

using namespace std;

FsmFactory::~FsmFactory() {
}

// Singleton, Not Multi-Thread Safe
FsmFactory &FsmFactory::Instance() {
	static FsmFactory instance;
	return instance;
}

int FsmFactory::AddStateMachine(int request_type, FsmCreate_t create_cb) {
	fsm_factory_.insert(make_pair(request_type, create_cb));
	return 0;
}

int FsmFactory::DelStateMachine(int request_type) {
	fsm_factory_.erase(request_type);
	return 0;
}

Fsm* FsmFactory::NewFsm(int request_type) {
	map<int, FsmCreate_t>::iterator it = fsm_factory_.find(request_type);
	if (it != fsm_factory_.end()) {
		return it->second();
	} else {
		return NULL;
	}
}
