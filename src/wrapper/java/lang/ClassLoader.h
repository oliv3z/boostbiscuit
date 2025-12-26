
#ifndef CLASSLOADER_H
#define CLASSLOADER_H

#include "java/classes/java_object.h"


class ClassLoader : java_object {
    static jclass class_loader_clazz;
    static inline jmethodID find_class_mid = nullptr;
    static inline jmethodID find_class_lunar_mid = nullptr;

public:
    ClassLoader(JNIEnv *env_, jobject class_loader_obj_);

    [[nodiscard]] jclass find_clazz(const std::string& class_path) const;

    [[nodiscard]] jclass find_lunar_clazz(const std::string& class_path) const;
};


#endif //CLASSLOADER_H
