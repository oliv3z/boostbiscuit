
#include "Entity.h"


std::shared_ptr<java_class> wrapper::Entity::Entity_class = nullptr;


wrapper::Entity::Entity(JNIEnv *env, jobject obj) : java_object(env, obj) {
    Entity_class = Entity_class ? Entity_class : std::make_shared<java_class>(env, "net/minecraft/entity/Entity");
}


std::shared_ptr<wrapper::AxisAlignedBB> wrapper::Entity::getEntityBoundingBox() const {
    if (!getEntityBoundingBox_mid)
        getEntityBoundingBox_mid = Entity_class->get_mid("getEntityBoundingBox", "()Lnet/minecraft/util/AxisAlignedBB;", false);

    jobject bbox = env->CallObjectMethod(obj, getEntityBoundingBox_mid);

    return std::make_shared<AxisAlignedBB>(env, bbox);
}


vec3 wrapper::Entity::get_pos() const {
    if (!posX_fid)
        posX_fid = Entity_class->get_field_id<jdouble>("posX", "D");
    if (!posY_fid)
        posY_fid = Entity_class->get_field_id<jdouble>("posY", "D");
    if (!posZ_fid)
        posZ_fid = Entity_class->get_field_id<jdouble>("posZ", "D");

    return { env->GetDoubleField(obj, posX_fid), env->GetDoubleField(obj, posY_fid), env->GetDoubleField(obj, posZ_fid) };
}


vec3 wrapper::Entity::get_prev_pos() const {
    if (!prev_posX_fid)
        prev_posX_fid = Entity_class->get_field_id<jdouble>("prevPosX", "D");
    if (!prev_posY_fid)
        prev_posY_fid = Entity_class->get_field_id<jdouble>("prevPosY", "D");
    if (!prev_posZ_fid)
        prev_posZ_fid = Entity_class->get_field_id<jdouble>("prevPosZ", "D");

    return { env->GetDoubleField(obj, prev_posX_fid), env->GetDoubleField(obj, prev_posY_fid), env->GetDoubleField(obj, prev_posZ_fid) };
}


bool wrapper::Entity::is_invisible() const {
    if (!isInvisible_mid)
        isInvisible_mid = Entity_class->get_mid("isInvisible", "()Z", false);

    return env->CallBooleanMethod(obj, isInvisible_mid);
}


float wrapper::Entity::get_distance_to_entity(const std::shared_ptr<Entity> &other) const {
    if (!getDistanceToEntity_mid)
        getDistanceToEntity_mid = Entity_class->get_mid("getDistanceToEntity", "(Lnet/minecraft/entity/Entity;)F", false);

    return env->CallFloatMethod(obj, getDistanceToEntity_mid, other->get_obj());
}
