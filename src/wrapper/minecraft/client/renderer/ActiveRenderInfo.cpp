
#include "ActiveRenderInfo.h"

#include <format>

#include "utils/logs/logger.h"


std::shared_ptr<java_class> wrapper::ActiveRenderInfo::ActiveRenderInfo_clazz = nullptr;


wrapper::ActiveRenderInfo::ActiveRenderInfo(JNIEnv *env, jobject obj) {
    this->env = env;
    this->obj = obj;
    ActiveRenderInfo_clazz = ActiveRenderInfo_clazz ? ActiveRenderInfo_clazz : std::make_shared<java_class>(env, "net/minecraft/client/renderer/ActiveRenderInfo");
}


std::vector<float> wrapper::ActiveRenderInfo::get_model_view(JNIEnv *env) {
    if (!ActiveRenderInfo_clazz)
        ActiveRenderInfo_clazz = std::make_shared<java_class>(env, "net/minecraft/client/renderer/ActiveRenderInfo");

    if (ActiveRenderInfo_clazz->get_clazz() == nullptr) {
        logger::get().error("where the fuck is my render info");
        return {};
    }

    if (!FloatBuffer_get_mid) {
        FloatBuffer_clazz = env->FindClass("java/nio/FloatBuffer");
        FloatBuffer_get_mid = env->GetMethodID(FloatBuffer_clazz, "get", "(I)F");
    }

    if (!MODELVIEW_fid)
        MODELVIEW_fid = ActiveRenderInfo_clazz->get_field_id<jobject>("MODELVIEW", "Ljava/nio/FloatBuffer;", true);

    const auto model_view_obj = env->GetStaticObjectField(ActiveRenderInfo_clazz->get_clazz(), MODELVIEW_fid);
    if (!model_view_obj)  return {};

    std::vector<float> model_view;
    for (int i = 0; i < 15; i++) {
        model_view.push_back(env->CallFloatMethod(model_view_obj, FloatBuffer_get_mid, i));
    }

    env->DeleteLocalRef(model_view_obj);

    return model_view;
}


std::vector<float> wrapper::ActiveRenderInfo::get_projection(JNIEnv *env) {
    if (!ActiveRenderInfo_clazz)
        ActiveRenderInfo_clazz = std::make_shared<java_class>(env, "net/minecraft/client/renderer/ActiveRenderInfo");

    if (ActiveRenderInfo_clazz->get_clazz() == nullptr) {
        logger::get().error("where the fuck is my render info");
        return {};
    }

    if (!FloatBuffer_get_mid) {
        FloatBuffer_clazz = env->FindClass("java/nio/FloatBuffer");
        FloatBuffer_get_mid = env->GetMethodID(FloatBuffer_clazz, "get", "(I)F");
    }

    if (!PROJECTION_fid)
        PROJECTION_fid = ActiveRenderInfo_clazz->get_field_id<jobject>("PROJECTION", "Ljava/nio/FloatBuffer;", true);

    const auto projection_obj = env->GetStaticObjectField(ActiveRenderInfo_clazz->get_clazz(), PROJECTION_fid);
    if (!projection_obj)  return {};

    std::vector<float> projection;
    for (int i = 0; i < 15; i++) {
        projection.push_back(env->CallFloatMethod(projection_obj, FloatBuffer_get_mid, i));
    }

    env->DeleteLocalRef(projection_obj);

    return projection;
}
