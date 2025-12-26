
#include "renderName_detour.h"

#include "hooks/java_hook.h"
#include "features/module/module_manager.h"
#include "features/module/modules/render/nametags.h"


void renderName_detour(hotspot::frame *frame, hotspot::thread *thread, bool *cancel) {
    if (!module_manager::initialized)
        return;

    auto module = (nametags *)module_manager::get().get_module("Nametags");
    if (module != nullptr && !module->is_enabled())
        return;

    JNIEnv *env = thread->get_env();

    //const auto entity_obj = get_object_param_at(frame, 1);
    //if (entity_obj == nullptr)
    //    return;

    // module->on_render_name(env, entity_obj, cancel);
}
