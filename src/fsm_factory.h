#ifndef _FSM_FACTORY_
#define _FSM_FACTORY_

#include <map>

#include "sock.h"
#include "fsm.h"

typedef Fsm* (*FsmCreate_t)(void);

class FsmFactory {
public:
	~FsmFactory();

	int AddStateMachine(int request_type, FsmCreate_t create_cb);

	int DelStateMachine(int request_type);

	Fsm* NewFsm(int request_type);

	static FsmFactory &Instance();

private:
	// Private Constructor For Singletion
	FsmFactory() {};

private:
	std::map<int, FsmCreate_t> fsm_factory_;
};

#define REFLECT_CREATE(classname, type) \
	static Fsm *classname##CreateMySelf() { \
		return new classname(); \
	} \
	static int ret __attribute__((unused)) = FsmFactory::Instance().AddStateMachine(type, classname##CreateMySelf);

#endif
