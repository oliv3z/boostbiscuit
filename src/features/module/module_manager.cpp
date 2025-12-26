
#include "module_manager.h"

#include <jni.h>
#include <thread>

#include "boostbiscuit.h"
#include "utils/logs/logger.h"
#include "modules/render/esp.h"
#include "java/jvm/jvm_manager.h"
#include "features/module/other/render_data.h"
#include "modules/render/nametags.h"
#include "register/class_register.h"
#include "wrapper/minecraft/client/Minecraft.h"


void module_manager::init() {
    // RENDER
    modules.push_back(std::make_unique<esp>());
    modules.push_back(std::make_unique<nametags>());

    // COMBAT

    // LEGIT

    // PLAYER

    // CLIENT


    // lazy xd
    for (const auto& module : modules) {
        module_map[module->get_name()] = module.get();
    }

    initialized = true;
}


std::vector<bb_module *> module_manager::get_modules() const {
    std::vector<bb_module*> result;
    for (const auto& module : modules) {
        result.push_back(module.get());
    }

    return result;
}


bb_module *module_manager::get_module(const std::string &name) const {
    const auto mod = module_map.find(name);
    return mod != module_map.end() ? mod->second : nullptr;
}


void module_manager::run_all_modules() {
    for (const auto &module : modules) {
        auto *m = module.get();
        std::thread([m] {
            JNIEnv *env;
            if (jvm_manager::get().get_jvm()->AttachCurrentThread((void **)&env, nullptr) != JNI_OK) {
                logger::get().error("Failed to attach current thread to JVM in module_manager::run_all_modules!");
                return;
            }

            while (bb::running.load()) {
                if (m->is_enabled()) {
                    env->PushLocalFrame(100);
                    m->run(env);
                    env->PopLocalFrame(nullptr);
                }

                Sleep(1);
            }

            jvm_manager::get().get_jvm()->DetachCurrentThread();
        }).detach();
    }

    std::thread([] {
        logger::get().info("Running render info thread...");
        while (bb::running.load()) {
            update_render_data();
            Sleep(1);
        }
    }).detach();
}


void module_manager::enable_defaults() const {
    get_module("ESP")->set_enabled(true);
    get_module("Nametags")->set_enabled(true);
}


void module_manager::WndProc_handler(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param) {
    JNIEnv *env;
    if (jvm_manager::get().get_jvm()->AttachCurrentThread((void **)&env, nullptr) != JNI_OK) {
        return logger::get().error("Failed to attach current thread to JVM in wglSwapBuffers_detour!");
    }

    const wrapper::Minecraft mc(env);
    if (!mc.is_current_screen_null())
        return;

    if (msg == WM_KEYDOWN) {
        const int key = static_cast<int>(w_param);
        for (const auto& module : modules) {
            if (module->keybind == key && module->is_keybind_holdable()) {
                module->set_enabled(true);
            } else if (module->keybind == key && !module->is_keybind_holdable()) {
                module->set_enabled(!module->is_enabled());
            }
        }
    } else if (msg == WM_KEYUP) {
        const int key = static_cast<int>(w_param);
        for (const auto& module : modules) {
            if (module->keybind == key && module->is_keybind_holdable()) {
                module->set_enabled(false);
            }
        }
    }
}
