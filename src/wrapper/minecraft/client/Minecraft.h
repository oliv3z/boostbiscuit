
#ifndef MINECRAFT_H
#define MINECRAFT_H

#include <memory>

#include "wrapper/wrapper.h"
#include "entity/EntityPlayerSP.h"
#include "gui/GuiScreen.h"
#include "multiplayer/WorldClient.h"
#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "renderer/entity/RenderManager.h"
#include "wrapper/minecraft/util/Timer.h"


BEGIN_WRAP

class Minecraft : public java_object {
    static std::shared_ptr<java_class> minecraft_class;
    static inline jfieldID theMinecraft_fid = nullptr;
    static inline jfieldID RenderManager_fid = nullptr;
    static inline jfieldID thePlayer_fid = nullptr;
    static inline jfieldID theWorld_fid = nullptr;
    static inline jfieldID timer_fid = nullptr;
    static inline jfieldID currentScreen_fid = nullptr;

public:
    explicit Minecraft(JNIEnv* env_);

    [[nodiscard]] std::shared_ptr<EntityPlayerSP> get_player() const;

    [[nodiscard]] std::shared_ptr<WorldClient> get_world() const;

    [[nodiscard]] std::shared_ptr<RenderManager> get_RenderManager() const;

    [[nodiscard]] std::shared_ptr<Timer> get_timer() const;

    [[nodiscard]] bool is_current_screen_null() const;
};

END_WRAP


#endif //MINECRAFT_H
