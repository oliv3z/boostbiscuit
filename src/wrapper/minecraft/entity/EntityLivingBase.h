
#ifndef ENTITYLIVINGBASE_H
#define ENTITYLIVINGBASE_H

#include <memory>

#include "Entity.h"
#include "java/classes/java_class.h"
#include "wrapper/wrapper.h"


BEGIN_WRAP

class EntityLivingBase : public Entity {
    static std::shared_ptr<java_class> EntityLivingBase_class;
    static inline jmethodID getHealth_mid = nullptr;
    static inline jmethodID getMaxHealth_mid = nullptr;
    static inline jmethodID isPlayer_mid = nullptr;

public:
    EntityLivingBase(JNIEnv* env, jobject obj);

    [[nodiscard]] float get_health() const;

    [[nodiscard]] float get_max_health() const;

    [[nodiscard]] bool is_player() const;
};

END_WRAP


#endif //ENTITYLIVINGBASE_H
