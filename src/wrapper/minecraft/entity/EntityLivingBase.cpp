
#include "EntityLivingBase.h"


std::shared_ptr<java_class> wrapper::EntityLivingBase::EntityLivingBase_class = nullptr;


wrapper::EntityLivingBase::EntityLivingBase(JNIEnv *env, jobject obj) : Entity(env, obj) {
    EntityLivingBase_class = EntityLivingBase_class ? EntityLivingBase_class : std::make_shared<java_class>(env, "net/minecraft/entity/EntityLivingBase");
}


float wrapper::EntityLivingBase::get_health() const {
    if (!getHealth_mid)
        getHealth_mid = EntityLivingBase_class->get_mid("getHealth", "()F", false);

    return env->CallFloatMethod(obj, getHealth_mid);
}


float wrapper::EntityLivingBase::get_max_health() const {
    if (!getMaxHealth_mid)
        getMaxHealth_mid = EntityLivingBase_class->get_mid("getMaxHealth", "()F", false);

    return env->CallFloatMethod(obj, getMaxHealth_mid);
}


bool wrapper::EntityLivingBase::is_player() const {
    if (!isPlayer_mid)
        isPlayer_mid = EntityLivingBase_class->get_mid("isPlayer", "()Z", false);

    return env->CallBooleanMethod(obj, isPlayer_mid);
}
