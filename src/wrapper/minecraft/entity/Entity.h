
#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "wrapper/wrapper.h"
#include "wrapper/minecraft/util/AxisAlignedBB.h"
#include "utils/structs/vec3.h"


BEGIN_WRAP

class Entity : public java_object {
    static std::shared_ptr<java_class> Entity_class;
    static inline jmethodID getEntityBoundingBox_mid;
    static inline jfieldID posX_fid = nullptr;
    static inline jfieldID posY_fid = nullptr;
    static inline jfieldID posZ_fid = nullptr;
    static inline jfieldID prev_posX_fid = nullptr;
    static inline jfieldID prev_posY_fid = nullptr;
    static inline jfieldID prev_posZ_fid = nullptr;
    static inline jmethodID isInvisible_mid = nullptr;
    static inline jmethodID getDistanceToEntity_mid = nullptr;

public:
    Entity(JNIEnv *env, jobject obj);

    [[nodiscard]] std::shared_ptr<AxisAlignedBB> getEntityBoundingBox() const;

    [[nodiscard]] vec3 get_pos() const;

    [[nodiscard]] vec3 get_prev_pos() const;

    [[nodiscard]] bool is_invisible() const;

    [[nodiscard]] float get_distance_to_entity(const std::shared_ptr<Entity> &other) const;
};

END_WRAP


#endif //ENTITY_H
