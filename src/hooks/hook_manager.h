
#ifndef HOOK_MANAGER_H
#define HOOK_MANAGER_H

#include <jni.h>
#include <vector>
#include <windows.h>

#include "hooks/java_hook.h"


#define BEGIN_HOOK_WRAP namespace hooks {
#define END_HOOK_WRAP }


BEGIN_HOOK_WRAP


struct hook_entry {
    void *original_function;
    void *target_function;
};


class hook_manager {
    inline static std::vector<hook_entry> hooks;

public:
    static void init();

    void cleanup();

    void setup_hooks();

    int register_hook(LPVOID target, LPVOID detour, LPVOID *original);

    int register_java_hook(jmethodID method_id, i2i_detour_t detour);

    static hook_manager& get() {
        static hook_manager instance;
        return instance;
    }
};

END_HOOK_WRAP


#endif //HOOK_MANAGER_H
