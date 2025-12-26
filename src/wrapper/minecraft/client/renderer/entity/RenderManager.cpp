
#include "RenderManager.h"


std::shared_ptr<java_class> wrapper::RenderManager::RenderManager_class = nullptr;
jfieldID wrapper::RenderManager::renderPosX_fid = nullptr;
jfieldID wrapper::RenderManager::renderPosY_fid = nullptr;
jfieldID wrapper::RenderManager::renderPosZ_fid = nullptr;


wrapper::RenderManager::RenderManager(JNIEnv *env, jobject obj) : java_object(env, obj) {
    RenderManager_class = RenderManager_class ? RenderManager_class : std::make_shared<java_class>(env, "net/minecraft/client/renderer/entity/RenderManager");
}


vec3 wrapper::RenderManager::get_render_pos() const {
    if (!renderPosX_fid || !renderPosY_fid || !renderPosZ_fid) {
        renderPosX_fid = RenderManager_class->get_field_id<double>("viewerPosX", "D", false);
        renderPosY_fid = RenderManager_class->get_field_id<double>("viewerPosY", "D", false);
        renderPosZ_fid = RenderManager_class->get_field_id<double>("viewerPosZ", "D", false);
    }

    const double x = env->GetDoubleField(obj, renderPosX_fid);
    const double y = env->GetDoubleField(obj, renderPosY_fid);
    const double z = env->GetDoubleField(obj, renderPosZ_fid);

    if (env->ExceptionCheck()) {
        logger::get().warn("error @ rendermanager");
        env->ExceptionDescribe();
        env->ExceptionClear();
        return {};
    }

    if (!x || !y || !z) {
        return { 420.f, 420.f, 420.f };
    }

    return { x, y, z };
}
