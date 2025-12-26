
#ifndef GUISCREEN_H
#define GUISCREEN_H

#include <memory>

#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "wrapper/wrapper.h"


BEGIN_WRAP

class GuiScreen : public java_object {
    static std::shared_ptr<java_class> GuiScreen_class;
    static std::shared_ptr<java_class> GuiChat_class;
    static std::shared_ptr<java_class> GuiInventory_class;
    static std::shared_ptr<java_class> GuiChest_class;

public:
    GuiScreen(JNIEnv *env, jobject obj);
};

END_WRAP


#endif //GUISCREEN_H
