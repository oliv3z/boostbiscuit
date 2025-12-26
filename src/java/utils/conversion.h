
#ifndef CONVERSION_H
#define CONVERSION_H

#include <jni.h>

inline jstring str_to_jstring(JNIEnv* env, const char *str) {
    return env->NewStringUTF(str);
}


#endif //CONVERSION_H
