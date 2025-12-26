
#include "WorldClient.h"


std::shared_ptr<java_class> wrapper::WorldClient::WorldClient_class = nullptr;


wrapper::WorldClient::WorldClient(JNIEnv *env, jobject obj) : World(env, obj) {
    WorldClient_class = WorldClient_class ? WorldClient_class : std::make_shared<java_class>(env, "net/minecraft/client/multiplayer/WorldClient");
}
