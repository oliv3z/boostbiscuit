
#ifndef NAMETAGS_H
#define NAMETAGS_H

#include "features/module/bb_module.h"
#include "features/module/module_manager.h"


class nametags : public bb_module {
public:
    nametags() : bb_module("Nametags", "Customize player nametags", bb_module_category::RENDER) { }

    static void on_render_name(JNIEnv *env, jobject entity, bool *cancel);  // this is exclusively called from the detour

    void on_imgui_render(const GLint viewport[4]) override;
};


#endif //NAMETAGS_H
