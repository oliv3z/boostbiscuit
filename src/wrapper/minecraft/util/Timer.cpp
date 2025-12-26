
#include "Timer.h"


std::shared_ptr<java_class> wrapper::Timer::Timer_class = nullptr;


wrapper::Timer::Timer(JNIEnv *env, jobject obj) : java_object(env, obj) {
    Timer_class = Timer_class ? Timer_class : std::make_shared<java_class>(env, "net/minecraft/util/Timer");
}


float wrapper::Timer::get_partial_ticks() const {
    if (!renderPartialTicks_mid)
        renderPartialTicks_mid = Timer_class->get_field_id<float>("renderPartialTicks", "F", false);

    return env->GetFloatField(obj, renderPartialTicks_mid);
}
