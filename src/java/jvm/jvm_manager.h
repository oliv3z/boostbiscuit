
#ifndef JVM_MANAGER_H
#define JVM_MANAGER_H

#include <jni.h>
#include <jvmti.h>

#include "wrapper/java/lang/ClassLoader.h"


enum class ClientBrand {
    VANILLA,
    LUNAR,
    BADLION,
    UNSUPPORTED
};


class jvm_manager {
    JavaVM *jvm;
    JNIEnv *env;
    jvmtiEnv *jvmti_env;

    ClassLoader *mc_class_loader;
    ClientBrand client_brand;

    jvm_manager() = default;
    ~jvm_manager();

public:
    bool initialize();

    [[nodiscard]] JavaVM *get_jvm() const;

    [[nodiscard]] JNIEnv *get_env() const;

    [[nodiscard]] jvmtiEnv *get_jvmti_env() const;

    [[nodiscard]] ClientBrand get_client_brand() const;

    [[nodiscard]] ClassLoader* get_mc_class_loader() const;

    static jvm_manager& get();
};


#endif //JVM_MANAGER_H
