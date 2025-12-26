
#ifndef RUNTICK_H
#define RUNTICK_H

#include "java/hotspot.h"


void runTick_detour(hotspot::frame* frame, hotspot::thread* thread, bool* cancel);


#endif //RUNTICK_H
