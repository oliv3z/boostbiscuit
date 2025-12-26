
#include "hook_manager.h"

#include <minhook.h>

#include "detours/java/renderName_detour.h"
#include "detours/java/renderWorldPass.h"
#include "java/jvm/jvm_manager.h"
#include "java/classes/java_class.h"
#include "hooks/detours/java/runTick.h"
#include "hooks/detours/native/wglSwapBuffers.h"


void hooks::hook_manager::init() {
    if (MH_STATUS status = MH_Initialize(); status != MH_OK) {
        return logger::get().error("Failed to initialize MinHook!");
    }

    constexpr jvmtiCapabilities capabilities{ .can_tag_objects = JVMTI_ENABLE, .can_retransform_classes = JVMTI_ENABLE };
    jvm_manager::get().get_jvmti_env()->AddCapabilities(&capabilities);

    hotspot::init();

    logger::get().info("Initialized hook manager!");
}


void hooks::hook_manager::cleanup() {
    clean_java_hooks();

    for (const auto &[original_function, target_function] : hooks) {
        MH_DisableHook(original_function);
        MH_RemoveHook(original_function);
    }
}


void hooks::hook_manager::setup_hooks() {
    // JAVA HOOKS

    /*
    const auto mc_class = java_class{ jvm_manager::get().get_env(), "net/minecraft/client/Minecraft" };
    jmethodID runTick_mid = mc_class.get_mid("runTick", "()V", false);
    if (register_java_hook(runTick_mid, runTick_detour) != 0)
        logger::get().error("Failed to register runTick detour!");
    else
        logger::get().info("Registered runTick detour!");
    */

    const auto entity_renderer_class = java_class{ jvm_manager::get().get_env(), "net/minecraft/client/renderer/EntityRenderer" };
    jmethodID renderWorldPass_mid = entity_renderer_class.get_mid("renderWorldPass", "(IFJ)V", false);
    if (register_java_hook(renderWorldPass_mid, renderWorldPass_detour) != 0)
        logger::get().error("Failed to register renderWorldPass detour!");
    else
        logger::get().info("Registered renderWorldPass detour!");

    const auto renderer_living_entity_class = java_class{ jvm_manager::get().get_env(), "net/minecraft/client/renderer/entity/RendererLivingEntity" };
    jmethodID renderName_mid = renderer_living_entity_class.get_mid("renderName", "(Lnet/minecraft/entity/EntityLivingBase;DDD)V", false);
    if (register_java_hook(renderName_mid, renderName_detour) != 0)
        logger::get().error("Failed to register renderName detour!");
    else
        logger::get().info("Registered renderName detour!");

    // NATIVE HOOKS

    const auto h_opengl32 = GetModuleHandleA("opengl32.dll");
    const auto p_wglSwapBuffers = GetProcAddress(h_opengl32, "wglSwapBuffers");
    if (register_hook((LPVOID)p_wglSwapBuffers, (LPVOID)&wglSwapBuffers::wglSwapBuffers_detour, (LPVOID *)&wglSwapBuffers::wglSwapBuffers_original) != 0) {
        logger::get().error("Failed to register wglSwapBuffers detour!");
    } else {
        logger::get().info("Registered wglSwapBuffers detour!");
    }
}


int hooks::hook_manager::register_java_hook(jmethodID method_id, const i2i_detour_t detour) {
    return create_java_hook(method_id, detour);
}


int hooks::hook_manager::register_hook(LPVOID target, LPVOID detour, LPVOID* original) {
    if (MH_STATUS status = MH_CreateHook(target, detour, original); status != MH_OK) {
        logger::get().error("Failed to create hook!");
        return 1;
    }

    if (MH_STATUS status = MH_EnableHook(target); status != MH_OK) {
        MH_RemoveHook(target);
        logger::get().error("Failed to enable hook!");
        return 1;
    }

    hooks.push_back({ target, original });

    return 0;
}
