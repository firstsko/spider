#ifndef _FSM_FACTORY_
#define _FSM_FACTORY_

#include <map>

#include "sock.h"
#include "fsm.h"

typedef Fsm *(*FsmCreate_t)();

class FsmFactory {
public:
	~FsmFactory();

	int AddStateMachine(int request_type, FsmCreate_t create_cb);

	int DelStateMachine(int request_type);

	static FsmFactory* Instance();

private:
	// Private Constructor For Singletion
	FsmFactory() {};

private:
	std::map <int, FsmCreate_t> fsm_factory_;
	static FsmFactory *pfactory_;
};

#define REFLECT_CREATE(ClassName, Id)			            			\
	static Fsm *ClassName##CreateMySelf() {								\
		return new ClassName();		                        			\
	}							                            			\
	FsmFactory::Instance()->AddStateMachine(Id, ClassName##CreateMySelf);

#endif
