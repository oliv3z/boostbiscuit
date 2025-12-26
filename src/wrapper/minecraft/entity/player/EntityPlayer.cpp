
#include "EntityPlayer.h"


std::shared_ptr<java_class> wrapper::EntityPlayer::EntityPlayer_class = nullptr;


wrapper::EntityPlayer::EntityPlayer(JNIEnv *env, jobject obj) : EntityLivingBase(env, obj) {
    EntityPlayer_class = EntityPlayer_class ? EntityPlayer_class : std::make_shared<java_class>(env, "net/minecraft/entity/player/EntityPlayer");
}
