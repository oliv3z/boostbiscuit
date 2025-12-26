
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <memory>

#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "wrapper/wrapper.h"
#include "utils/structs/vec3.h"


BEGIN_WRAP

class RenderManager : public java_object {
    static std::shared_ptr<java_class> RenderManager_class;
    static jfieldID renderPosX_fid;
    static jfieldID renderPosY_fid;
    static jfieldID renderPosZ_fid;

public:
    RenderManager(JNIEnv *env, jobject obj);

    [[nodiscard]] vec3 get_render_pos() const;
};

END_WRAP


#endif //RENDERMANAGER_H
