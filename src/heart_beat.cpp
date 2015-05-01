#include "heart_beat.h"
#include "fsm_factory.h"

using namespace spider;

static const int request_type = HEART_BEAT_REQUEST;

REFLECT_CREATE(HeartBeat, request_type);		            			\

HeartBeat::HeartBeat() {
}

HeartBeat::~HeartBeat() {
}
