#ifndef _HEART_BEAT_
#define _HEART_BEAT_

#include "fsm.h"

class HeartBeat:public Fsm {
public:
	HeartBeat();

	virtual ~HeartBeat();

	virtual int FsmType();

	Status_t AliveResponse(void *smessage);

	typedef enum {
		INIT_STATE = 0, 
	} State_t;

private:
	
};

#endif
