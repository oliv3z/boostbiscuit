
#ifndef WORLDCLIENT_H
#define WORLDCLIENT_H

#include <memory>

#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "wrapper/wrapper.h"
#include "wrapper/minecraft/world/World.h"


BEGIN_WRAP
    class WorldClient : public World {
    static std::shared_ptr<java_class> WorldClient_class;

public:
    WorldClient(JNIEnv *env, jobject obj);
};

END_WRAP


#endif //WORLDCLIENT_H
