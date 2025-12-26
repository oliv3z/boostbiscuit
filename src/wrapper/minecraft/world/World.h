
#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>

#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "wrapper/wrapper.h"
#include "wrapper/minecraft/entity/player/EntityPlayer.h"


BEGIN_WRAP
    class World : public java_object {
    static std::shared_ptr<java_class> World_class;
    static inline jfieldID playerEntities_fid = nullptr;

public:
    World(JNIEnv *env, jobject obj);

    [[nodiscard]] std::vector<std::shared_ptr<EntityPlayer>> get_players() const;
};

END_WRAP


#endif //WORLD_H
