
#include "EntityPlayerSP.h"


std::shared_ptr<java_class> wrapper::EntityPlayerSP::EntityPlayerSP_class = nullptr;


wrapper::EntityPlayerSP::EntityPlayerSP(JNIEnv *env, jobject obj) : AbstractClientPlayer(env, obj) {
    EntityPlayerSP_class = EntityPlayerSP_class ? EntityPlayerSP_class : std::make_shared<java_class>(env, "net/minecraft/client/entity/EntityPlayerSP");
}
