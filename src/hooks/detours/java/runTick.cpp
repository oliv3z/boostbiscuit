
#include "runTick.h"

#include "wrapper/minecraft/client/Minecraft.h"


void runTick_detour(hotspot::frame *frame, hotspot::thread *thread, bool *cancel) {
    logger::get().info("runTick detour");
}
