
#ifndef ABSTRACTCLIENTPLAYER_H
#define ABSTRACTCLIENTPLAYER_H


#include "java/classes/java_class.h"
#include "wrapper/minecraft/entity/player/EntityPlayer.h"
#include "wrapper/wrapper.h"

BEGIN_WRAP

class AbstractClientPlayer : public EntityPlayer {
    static std::shared_ptr<java_class> AbstractClientPlayer_class;

public:
    AbstractClientPlayer(JNIEnv *env, jobject obj);
};

END_WRAP


#endif //ABSTRACTCLIENTPLAYER_H
