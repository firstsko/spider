#ifndef _FSM_FACTORY_
#define _FSM_FACTORY_

#include <map>

#include "sock.h"
#include "fsm.h"

class FsmFactory {
public:
	~FsmFactory();

	int AddStateMachine(int message_type, Fsm *pfsm);

	int ActivateCb(SMessage *pmessage, int message_type, int dst_state);
	
	int DelStateMachine(int message_type);

	static FsmFactory* Instance();

private:
	// Private Constructor For Singletion
	FsmFactory() {};

private:
	std::map <int, Fsm *> fsm_factory_;
};

#define DECLARE_CLASS_CREATE(class_name) \ 
static CObject* CreateClass## class_name (); 

#define IMPL_CLASS_CREATE(class_name) \ 
static CObject* CreateClass## class_name (){  \ 
       return new class_name;             \ 
}; 

#define REG_CLASS_CREATE(class_name) \ 
RegisterFactoryCreate(class_name::CreateClass## class_name, #class_name); 

#endif
