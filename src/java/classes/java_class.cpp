
#include "java_class.h"

#include "utils/logs/logger.h"
#include "mapping/mapping.h"
#include "java/jvm/jvm_manager.h"


java_class::java_class(JNIEnv *env_, const std::string &class_path_) {
    env = env_;
    class_path = class_path_;

    const std::string cp = mapping::get_class_path(class_path);

    jclass local_clazz = nullptr;
    if (!cp.empty())
        local_clazz = jvm_manager::get().get_client_brand() == ClientBrand::LUNAR
            ? jvm_manager::get().get_mc_class_loader()->find_lunar_clazz(cp)
            : jvm_manager::get().get_mc_class_loader()->find_clazz(cp);

    if (local_clazz == nullptr) {
        local_clazz = env->FindClass(cp.c_str());
        if (env->ExceptionCheck())
            env->ExceptionClear();
    }
    if (local_clazz == nullptr) {  // most likely a core java class like java/lang/String
        local_clazz = env->FindClass(class_path.c_str());
        if (env->ExceptionCheck())
            env->ExceptionClear();
    }
    if (local_clazz == nullptr)
        logger::get().warn("Class " + class_path + " still not found at java_class constructor!");

    clazz = make_global(local_clazz);
    env->DeleteLocalRef(local_clazz);
}


java_class::~java_class() {
    if (env && clazz)
        env->DeleteGlobalRef(clazz);
}


jclass java_class::get_clazz() const {
    return clazz;
}


jclass java_class::make_global(jclass obj_) const {
    if (obj_)
        return (jclass)env->NewGlobalRef(obj_);

    logger::get().warn("Trying to make global ref of null object! (java_class)");
    return nullptr;
}


jmethodID java_class::get_mid(const std::string &method_name, const std::string &method_signature, bool is_static) const {
    std::string obf_method_name;
    std::string obf_method_signature;

    if (class_path.substr(0, 13) == "net/minecraft")
        std::tie(obf_method_name, obf_method_signature) = mapping::get_method_name_and_signature(class_path + "/" + method_name, method_signature);
    else
        std::tie(obf_method_name, obf_method_signature) = std::make_pair(method_name, method_signature);

    jmethodID method_id;
    if (is_static)
        method_id = env->GetStaticMethodID(clazz, obf_method_name.c_str(), obf_method_signature.c_str());
    else
        method_id = env->GetMethodID(clazz, obf_method_name.c_str(), obf_method_signature.c_str());

    if (!method_id) {
        logger::get().error("Failed to get method ID for " + method_name + " with signature " + method_signature);
        env->ExceptionDescribe();
        env->ExceptionClear();
        logger::get().error("Method " + method_name + " with signature " + method_signature + " not found in class");
    }

    return method_id;
}
