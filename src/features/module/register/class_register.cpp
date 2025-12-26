
#include "class_register.h"


std::unordered_map<std::thread::id, game_classes> class_register::registered_classes{};
std::mutex class_register::registered_classes_mutex;


game_classes& class_register::get_thread_game_classes(JNIEnv* env) {
    const auto tid = std::this_thread::get_id();

    std::lock_guard lock(registered_classes_mutex);
    auto& gc = registered_classes[tid];

    if (!gc.minecraft || gc.minecraft->is_null()) {
        gc.minecraft = std::make_shared<wrapper::Minecraft>(env);
        gc.local_player = gc.minecraft->get_player();
        gc.world = gc.minecraft->get_world();
    }

    return gc;
}


// TODO: find out if the map is redundant and we should just not store it
bool class_register::safety_check_and_get_thread_game_classes(JNIEnv* env, game_classes* p_game_classes) {
    if (!p_game_classes) {
        logger::get().warn("someone brought a nullptr to class register?!");
        return false;
    }

    game_classes& gc = get_thread_game_classes(env);

    std::lock_guard lock(registered_classes_mutex);


    auto new_mc = std::make_shared<wrapper::Minecraft>(env);
    if (!new_mc || new_mc->is_null()) {
        logger::get().info("new mc null");
        return false;
    }

    auto new_world = new_mc->get_world();
    if (!new_world || new_world->is_null()) {
        logger::get().info("new world null");
        return false;
    }

    auto new_player = new_mc->get_player();
    if (!new_player || new_player->is_null()) {
        logger::get().info("new player null");
        return false;
    }

    game_classes new_gc = { new_mc, new_player, new_world};
    registered_classes[std::this_thread::get_id()] = new_gc;

    *p_game_classes = new_gc;
    return true;
}
