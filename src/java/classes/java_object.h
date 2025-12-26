
#ifndef JAVA_OBJECT_H
#define JAVA_OBJECT_H

#include <jni.h>

#include "utils/logs/logger.h"


class java_object {
protected:
    JNIEnv* env = nullptr;
    jobject obj = nullptr;

    java_object() = default;  // cannot be instantiated directly

    java_object(JNIEnv *env_, jobject obj_, bool is_mc = false) {
        env = env_;
        if (!is_mc)
            obj = make_global(obj_);
    }

    jobject make_global(jobject obj_) const {
        jobject global_ref = nullptr;
        if (obj_)
            global_ref = env->NewGlobalRef(obj_);
        else
            logger::get().warn("Trying to make global ref of null object! (java_object)");

        env->DeleteLocalRef(obj_);  // NOTE: this function will delete local ref!! only use global ref after calling

        return global_ref;
    }

public:
    ~java_object() {
        if (env && obj)
            env->DeleteGlobalRef(obj);
    }

    [[nodiscard]] jobject get_obj() const { return obj; }

    void refresh_obj(jobject new_obj, const bool is_already_global = false) {
        if (obj)
            env->DeleteGlobalRef(obj);
        obj = is_already_global ? obj : make_global(new_obj);
    }

    [[nodiscard]] bool is_null() const { return env->IsSameObject(obj, nullptr); }
};


#endif //JAVA_OBJECT_H
