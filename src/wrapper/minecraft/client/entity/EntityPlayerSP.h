
#ifndef ENTITYPLAYERSP_H
#define ENTITYPLAYERSP_H

#include "AbstractClientPlayer.h"
#include "java/classes/java_class.h"
#include "wrapper/wrapper.h"


BEGIN_WRAP

class EntityPlayerSP : public AbstractClientPlayer {
    static std::shared_ptr<java_class> EntityPlayerSP_class;

public:
    EntityPlayerSP(JNIEnv *env, jobject obj);
};

END_WRAP


#endif //ENTITYPLAYERSP_H
