
#ifndef ENTITYPLAYER_H
#define ENTITYPLAYER_H

#include <memory>

#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "wrapper/wrapper.h"
#include "wrapper/minecraft/entity/EntityLivingBase.h"


BEGIN_WRAP

class EntityPlayer : public EntityLivingBase {
    static std::shared_ptr<java_class> EntityPlayer_class;

public:
    EntityPlayer(JNIEnv *env, jobject obj);
};

END_WRAP


#endif //ENTITYPLAYER_H
