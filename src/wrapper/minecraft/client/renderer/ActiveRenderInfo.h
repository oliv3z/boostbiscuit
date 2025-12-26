
#ifndef ACTIVERENDERINFO_H
#define ACTIVERENDERINFO_H

#include <memory>
#include <vector>

#include "wrapper/wrapper.h"
#include "java/classes/java_class.h"
#include "java/classes/java_object.h"


BEGIN_WRAP

class ActiveRenderInfo : java_object {
    static std::shared_ptr<java_class> ActiveRenderInfo_clazz;
    static inline jclass FloatBuffer_clazz = nullptr;
    static inline jmethodID FloatBuffer_get_mid = nullptr;
    static inline jfieldID MODELVIEW_fid = nullptr;
    static inline jfieldID PROJECTION_fid = nullptr;

public:
    ActiveRenderInfo(JNIEnv *env, jobject obj);

    static std::vector<float> get_model_view(JNIEnv *env);

    static std::vector<float> get_projection(JNIEnv *env);
};

END_WRAP


#endif //ACTIVERENDERINFO_H
