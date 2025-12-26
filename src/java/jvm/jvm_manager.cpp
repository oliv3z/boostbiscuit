
#include "jvm_manager.h"

#include <windows.h>

#include "java/utils/conversion.h"
#include "wrapper/java/lang/ClassLoader.h"


jvm_manager::~jvm_manager() {
    if (jvm)
        jvm->DetachCurrentThread();

    if (jvmti_env) {
        jvmti_env->DisposeEnvironment();
    }
}


typedef jint (JNICALL *JNI_GetCreatedJavaVMs_t)(JavaVM**, jsize, jsize*);


JavaVM *get_java_vm() {
    const HMODULE h_jvm_dll = GetModuleHandle("jvm.dll");
    if (!h_jvm_dll)  return nullptr;

    const auto JNI_GetCreatedJavaVMs = (JNI_GetCreatedJavaVMs_t) GetProcAddress(h_jvm_dll, "JNI_GetCreatedJavaVMs");
    if (!JNI_GetCreatedJavaVMs) return nullptr;

    JavaVM* jvm;
    jsize vm_count;

    if (JNI_GetCreatedJavaVMs(&jvm, 1, &vm_count) != JNI_OK || vm_count == 0) {
        return nullptr;
    }

    return jvm;
}


/* Get all loaded classes via JVMTI and find Minecraft class by name
 * -> Get it's class loader or return nullptr if not found
*/
ClassLoader *find_mc_class_loader(jvmtiEnv *jvmti_env, JNIEnv *env) {
    jint loaded_classes_count = 0;
    jclass* loaded_classes = nullptr;

    jvmti_env->GetLoadedClasses(&loaded_classes_count, &loaded_classes);

    jclass class_clazz = env->FindClass("java/lang/Class");
    jmethodID get_clazz_name_mid = env->GetMethodID(class_clazz, "getName", "()Ljava/lang/String;");
    env->DeleteLocalRef(class_clazz);

    jobject class_loader_obj = nullptr;
    for (jint i = 0; i < loaded_classes_count; i++) {
        jclass clazz = loaded_classes[i];

        const auto clazz_name_obj = (jstring)env->CallObjectMethod(clazz, get_clazz_name_mid);
        const char* clazz_name = env->GetStringUTFChars(clazz_name_obj, nullptr);
        env->ReleaseStringUTFChars(clazz_name_obj, clazz_name);

        if (strcmp(clazz_name, "net.minecraft.client.Minecraft") != 0 && strcmp(clazz_name, "ave") != 0)
            continue;

        jvmti_env->GetClassLoader(clazz, &class_loader_obj);
        break;
    }

    jvmti_env->Deallocate((unsigned char *)loaded_classes);

    return class_loader_obj == nullptr ? nullptr : new ClassLoader(env, class_loader_obj);
}


ClientBrand find_client_brand(JNIEnv* env) {
    if (env->FindClass("com/moonsworth/lunar/genesis/Genesis") != nullptr) {
        logger::get().info("Lunar client detected!");
        return ClientBrand::LUNAR;
    }
    if (env->ExceptionCheck())  env->ExceptionClear();

    if (env->FindClass("net/badlion/client/a") != nullptr) {
        logger::get().info("Badlion client detected!");
        return ClientBrand::BADLION;
    }
    if (env->ExceptionCheck())  env->ExceptionClear();

    if (env->FindClass("adm") != nullptr) {  // adm = net/minecraft/client/World btw
        logger::get().info("Vanilla client detected!");
        return ClientBrand::VANILLA;
    }
    if (env->ExceptionCheck())  env->ExceptionClear();

    logger::get().error("Unsupported client detected!!!");
    return ClientBrand::UNSUPPORTED;
}


bool jvm_manager::initialize() {
    if ((jvm = get_java_vm()) == nullptr)
        return false;

    JNIEnv *env_;
    jvm->AttachCurrentThread((void**)&env_, nullptr);
    if (env_ == nullptr)
        return false;

    jvmtiEnv *jvmti_env_;
    jvm->GetEnv((void**)&jvmti_env_, JVMTI_VERSION_1_2);
    if (!jvmti_env_)
        return false;

    env = env_;
    jvmti_env = jvmti_env_;

    ClassLoader *mc_class_loader_ = find_mc_class_loader(jvmti_env_, env_);
    if (!mc_class_loader_) {
        logger::get().error("Failed to get Minecraft class loader!");
        return false;
    }

    mc_class_loader = mc_class_loader_;

    client_brand = find_client_brand(env_);
    if (client_brand == ClientBrand::UNSUPPORTED)
        return false;

    return true;
}


JavaVM *jvm_manager::get_jvm() const {
    return jvm;
}


JNIEnv *jvm_manager::get_env() const {
    return env;
}


jvmtiEnv *jvm_manager::get_jvmti_env() const {
    return jvmti_env;
}


ClientBrand jvm_manager::get_client_brand() const {
    return client_brand;
}


ClassLoader *jvm_manager::get_mc_class_loader() const {
    return mc_class_loader;
}


jvm_manager& jvm_manager::get() {
    static jvm_manager instance;
    return instance;
}
