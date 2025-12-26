
#include "renderWorldPass.h"

#include "features/module/module_manager.h"
#include "hooks/java_hook.h"


void renderWorldPass_detour(hotspot::frame *frame, hotspot::thread *thread, bool *cancel) {
    JNIEnv *env = thread->get_env();

    const auto pass = get_primitive_param_at<jint>(frame, 1);
    const auto partial_ticks = get_primitive_param_at<jfloat>(frame, 2);
    const auto finish_time_nano = get_primitive_param_at<jlong>(frame, 3);

    for (const auto &module : module_manager::get().get_modules()) {
        if (module->is_enabled()) {
            env->PushLocalFrame(100);
            module->on_renderWorldPass(env, pass, partial_ticks, finish_time_nano);
            env->PopLocalFrame(nullptr);
        }
    }
}
