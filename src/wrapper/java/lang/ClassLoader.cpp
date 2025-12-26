
#include "ClassLoader.h"

#include "java/utils/conversion.h"


jclass ClassLoader::class_loader_clazz = nullptr;


ClassLoader::ClassLoader(JNIEnv *env_, jobject class_loader_obj_) {
    env = env_;
    obj = class_loader_obj_;

    if (!class_loader_clazz) {
        jclass result = env->FindClass("java/lang/ClassLoader");
        class_loader_clazz = (jclass)env->NewGlobalRef(result);
        env->DeleteLocalRef(result);
    }
}


jclass ClassLoader::find_clazz(const std::string &class_path) const {
    if (!find_class_mid)
        find_class_mid = env->GetMethodID(class_loader_clazz, "findLoadedClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring class_path_jstring = str_to_jstring(env, class_path.c_str());
    auto result = (jclass)env->CallObjectMethod(this->obj, find_class_mid, class_path_jstring);
    env->DeleteLocalRef(class_path_jstring);

    if (!result)  return nullptr;

    return result;
}


jclass ClassLoader::find_lunar_clazz(const std::string &class_path) const {
    if (!find_class_lunar_mid)
        find_class_lunar_mid = env->GetMethodID(class_loader_clazz, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring class_path_jstring = str_to_jstring(env, class_path.c_str());
    auto result = (jclass)env->CallObjectMethod(this->obj, find_class_lunar_mid, class_path_jstring);
    env->DeleteLocalRef(class_path_jstring);

    if (!result)  return nullptr;

    return result;
}
