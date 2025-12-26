
#include "Minecraft.h"


std::shared_ptr<java_class> wrapper::Minecraft::minecraft_class = nullptr;


wrapper::Minecraft::Minecraft(JNIEnv* env_) : java_object(env_, nullptr, true) {
    minecraft_class = minecraft_class ? minecraft_class : std::make_shared<java_class>(env, "net/minecraft/client/Minecraft");

    if (!theMinecraft_fid)
        theMinecraft_fid = minecraft_class->get_field_id<jobject>("theMinecraft", "Lnet/minecraft/client/Minecraft;", true);

    jobject theMinecraft_obj = env->GetStaticObjectField(minecraft_class->get_clazz(), theMinecraft_fid);
    this->obj = make_global(theMinecraft_obj);
}


std::shared_ptr<wrapper::EntityPlayerSP> wrapper::Minecraft::get_player() const {
    if (!thePlayer_fid)
        thePlayer_fid = minecraft_class->get_field_id<jobject>("thePlayer", "Lnet/minecraft/client/entity/EntityPlayerSP;");

    const auto thePlayerJObjectRef = env->NewGlobalRef(env->GetObjectField(obj, thePlayer_fid));

    return std::make_shared<EntityPlayerSP>(env, thePlayerJObjectRef);
}


std::shared_ptr<wrapper::WorldClient> wrapper::Minecraft::get_world() const {
    if (!theWorld_fid)
        theWorld_fid = minecraft_class->get_field_id<jobject>("theWorld", "Lnet/minecraft/client/multiplayer/WorldClient;");

    const auto theWorldJObjectRef = env->NewGlobalRef(env->GetObjectField(obj, theWorld_fid));

    return std::make_shared<WorldClient>(env, theWorldJObjectRef);
}


std::shared_ptr<wrapper::RenderManager> wrapper::Minecraft::get_RenderManager() const {
    if (!RenderManager_fid)
        RenderManager_fid = minecraft_class->get_field_id<jobject>("renderManager", "Lnet/minecraft/client/renderer/entity/RenderManager;", false);

    return std::make_shared<RenderManager>(env, env->GetObjectField(obj, RenderManager_fid));
}


std::shared_ptr<wrapper::Timer> wrapper::Minecraft::get_timer() const {
    if (!timer_fid)
        timer_fid = minecraft_class->get_field_id<jobject>("timer", "Lnet/minecraft/util/Timer;", false);

    return std::make_shared<Timer>(env, env->GetObjectField(obj, timer_fid));
}


bool wrapper::Minecraft::is_current_screen_null() const {
    if (!currentScreen_fid)
        currentScreen_fid = minecraft_class->get_field_id<jobject>("currentScreen", "Lnet/minecraft/client/gui/GuiScreen;", false);

    const auto currentScreen_obj = env->GetObjectField(obj, currentScreen_fid);
    return currentScreen_obj == nullptr;
}
