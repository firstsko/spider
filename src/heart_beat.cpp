#include "fsm_factory.h"
#include "heart_beat.h"

using namespace spider;

static const int request_type = HEART_BEAT_REQUEST;

REFLECT_CREATE(HeartBeat, request_type);
STATE_CALLBACK(HEART_BEAT_REQUEST, INIT_STATE, HeartBeat, AliveResponse);

HeartBeat::HeartBeat() {
}

int HeartBeat::FsmType() {
	return request_type;
}

Status_t HeartBeat::AliveResponse(void *smessage) {
	INFO("I Am OKay");


	return FSM_FINISH;
}

HeartBeat::~HeartBeat() {

}
