
#ifndef RENDERGLOBAL_H
#define RENDERGLOBAL_H

#include <memory>

#include "java/classes/java_object.h"
#include "java/classes/java_class.h"
#include "wrapper/minecraft/util/AxisAlignedBB.h"


BEGIN_WRAP

class RenderGlobal : java_object {
    static std::shared_ptr<java_class> RenderGlobal_class;
    static jmethodID drawSelectionBoundingBox_mid;

public:
    RenderGlobal(JNIEnv* env, jobject obj);

    static void drawSelectionBoundingBox(JNIEnv *env, const std::shared_ptr<AxisAlignedBB> &bbox);
};

END_WRAP


#endif //RENDERGLOBAL_H
