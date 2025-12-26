
#ifndef UPDATERENDERINFO_H
#define UPDATERENDERINFO_H

#include "java/hotspot.h"


void updateRenderInfo_detour(hotspot::frame *frame, hotspot::thread *thread, bool *cancel);


#endif //UPDATERENDERINFO_H
