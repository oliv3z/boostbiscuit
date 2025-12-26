
#ifndef CLASS_REGISTER_H
#define CLASS_REGISTER_H

// I got this idea from raid0source on github
// it's a good optimization speed and safety wise
// on a second thought this might be fucking redundant since java objects are thread-agnostic
// meh

#include <memory>
#include <unordered_map>
#include <mutex>
#include <thread>

#include "wrapper/minecraft/client/Minecraft.h"


// every class inside this struct should have an is_valid() method to check if its object is still valid, not is_null()
// since java objects can become invalid if e.g. world is left
struct game_classes {
    std::shared_ptr<wrapper::Minecraft> minecraft;
    std::shared_ptr<wrapper::EntityPlayerSP> local_player;
    std::shared_ptr<wrapper::WorldClient> world;
};


class class_register {
    static std::unordered_map<std::thread::id, game_classes> registered_classes;
    static std::mutex registered_classes_mutex;

public:
    static game_classes& get_thread_game_classes(JNIEnv* env);
    static bool safety_check_and_get_thread_game_classes(JNIEnv* env, game_classes* p_game_classes);
};


#endif //CLASS_REGISTER_H
