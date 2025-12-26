
#ifndef ESP_H
#define ESP_H

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"

#include "features/module/bb_module.h"
#include "features/module/module_manager.h"


class esp : public bb_module {
public:
    enum enum_esp_mode {
        RECTANGLE = 0,
        HITBOX = 1
    };

    static inline int render_mode = RECTANGLE;

    static inline bool only_render_invisible = false;

    static inline float color[4] = { 1.f, 1.f, 1.f, 1.f };

    static inline bool render_health_bar = true;

    static inline unsigned int max_distance_value = 400.f;
    static constexpr unsigned int min_distance = 30.f, max_distance = 500.f;

    esp() : bb_module("ESP", "See players through walls", bb_module_category::RENDER) { }

    void on_imgui_render(const GLint viewport[4]) override;

    void on_swap_buffers(JNIEnv *env) override;
};


#endif //ESP_H
