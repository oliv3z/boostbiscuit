
#include "RenderGlobal.h"


std::shared_ptr<java_class> wrapper::RenderGlobal::RenderGlobal_class = nullptr;
jmethodID wrapper::RenderGlobal::drawSelectionBoundingBox_mid = nullptr;


wrapper::RenderGlobal::RenderGlobal(JNIEnv* env, jobject obj) : java_object(env, obj) {
    RenderGlobal_class = RenderGlobal_class ? RenderGlobal_class : std::make_shared<java_class>(env, "net/minecraft/client/renderer/RenderGlobal");
}


void wrapper::RenderGlobal::drawSelectionBoundingBox(JNIEnv *env, const std::shared_ptr<AxisAlignedBB> &bbox) {
    if (!RenderGlobal_class || !RenderGlobal_class->get_clazz())
        RenderGlobal_class = std::make_shared<java_class>(env, "net/minecraft/client/renderer/RenderGlobal");

    if (!drawSelectionBoundingBox_mid)
        drawSelectionBoundingBox_mid = RenderGlobal_class->get_mid("drawSelectionBoundingBox", "(Lnet/minecraft/util/AxisAlignedBB;)V", true);

    env->CallStaticVoidMethod(RenderGlobal_class->get_clazz(), drawSelectionBoundingBox_mid, bbox->get_obj());
}
