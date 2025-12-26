
#ifndef BB_MODULE_H
#define BB_MODULE_H

#include <GL/gl.h>
#include <jni.h>
#include <string>

enum class bb_module_category;


class bb_module {
    const std::string name;
    const std::string description;
    const bb_module_category category;
    const bool keybind_holdable;

public:
    bool enabled = false;
    int keybind = 0;

    bb_module(std::string name, std::string description, const bb_module_category category, const bool keybind_holdable = false)
        : name(std::move(name)), description(std::move(description)), category(category), keybind_holdable(keybind_holdable) {}

    [[nodiscard]] const std::string& get_name() const {
        return name;
    }

    [[nodiscard]] const std::string& get_description() const {
        return description;
    }

    [[nodiscard]] bb_module_category get_category() const {
        return category;
    }

    [[nodiscard]] bool is_enabled() const {
        return enabled;
    }

    void set_enabled(const bool enabled_) {
        enabled = enabled_;
    }

    bool is_keybind_holdable() const {
        return keybind_holdable;
    }

    virtual void run(JNIEnv *env) {}

    virtual void on_imgui_render(const GLint viewport[4]) {}

    virtual void on_swap_buffers(JNIEnv *env) {}

    virtual void on_renderWorldPass(JNIEnv *env, jint pass, jfloat partial_ticks, jlong finish_time_nano) {}

    virtual ~bb_module() = default;
};


#endif //BB_MODULE_H
