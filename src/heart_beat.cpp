#include "fsm_factory.h"
#include "heart_beat.h"

using namespace spider;

static const int request_type = HEART_BEAT_REQUEST;

/*static Fsm* HeartBeatCreateMySelf() {										
	return new HeartBeat();
}
static int ret __attribute__((unused)) = FsmFactory::Instance()->AddStateMachine(request_type, HeartBeatCreateMySelf); */

REFLECT_CREATE(HeartBeat, request_type);

HeartBeat::HeartBeat() {
}

HeartBeat::~HeartBeat() {
}
