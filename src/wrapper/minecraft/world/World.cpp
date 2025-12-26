
#include "World.h"

#include <vector>

#include "wrapper/java/utils/List.h"


std::shared_ptr<java_class> wrapper::World::World_class = nullptr;


wrapper::World::World(JNIEnv *env, jobject obj) : java_object(env, obj) {
    World_class = World_class ? World_class : std::make_shared<java_class>(env, "net/minecraft/world/World");
}


std::vector<std::shared_ptr<wrapper::EntityPlayer>> wrapper::World::get_players() const {
    if (!playerEntities_fid)
        playerEntities_fid = World_class->get_field_id<jobject>("playerEntities", "Ljava/util/List;");

    // TODO: ADD MonitorEnter/Exit for thread safety

    jobject playerEntities_list_obj = env->GetObjectField(this->get_obj(), playerEntities_fid);
    if (env->ExceptionCheck()) {
        logger::get().warn("error @ world (1)");
        env->ExceptionDescribe();
        env->ExceptionClear();
        return {};
    }

    const auto playerEntities_list = List(env, playerEntities_list_obj);
    if (env->ExceptionCheck()) {
        logger::get().warn("error @ world (2)");
        env->ExceptionDescribe();
        env->ExceptionClear();
        return {};
    }

    jobjectArray playerEntities_jobjects = playerEntities_list.to_array();
    if (!playerEntities_jobjects)  return {};

    jsize length = env->GetArrayLength(playerEntities_jobjects);

    std::vector<std::shared_ptr<EntityPlayer>> players;
    players.reserve(length);
    for (jsize i = 0; i < length; ++i) {
        jobject player = env->GetObjectArrayElement(playerEntities_jobjects, i);

        if (player)
            players.push_back(std::make_shared<EntityPlayer>(env, player));
    }

    env->DeleteLocalRef(playerEntities_jobjects);

    return players;
}
