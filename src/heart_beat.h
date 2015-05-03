#ifndef _HEART_BEAT_
#define _HEART_BEAT_

#include "fsm.h"

class HeartBeat:public Fsm {
public:
	HeartBeat();

	Status_t AliveResponse(void *smessage);

	~HeartBeat();

	typedef enum {
		INIT_STATE = 0, 
	} State_t;

private:
	
};

#endif
