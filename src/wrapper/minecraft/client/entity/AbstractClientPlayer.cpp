
#include "AbstractClientPlayer.h"


std::shared_ptr<java_class> wrapper::AbstractClientPlayer::AbstractClientPlayer_class = nullptr;


wrapper::AbstractClientPlayer::AbstractClientPlayer(JNIEnv *env, jobject obj) : EntityPlayer(env, obj) {
    AbstractClientPlayer_class = AbstractClientPlayer_class ? AbstractClientPlayer_class : std::make_shared<java_class>(env, "net/minecraft/client/entity/AbstractClientPlayer");
}
