
#ifndef JAVA_HOOK_H
#define JAVA_HOOK_H

#include <jni.h>
#include <vector>
#include <cstdint>

#include "java/hotspot.h"


typedef void (*i2i_detour_t)(hotspot::frame *frame, hotspot::thread *thread, bool *cancel);


class java_hook {
    uint8_t *target = nullptr;
    uint8_t *allocated_assembly = nullptr;

public:
    java_hook(uint8_t *target, i2i_detour_t detour);
    ~java_hook();

    bool is_error = true;
};


struct i2i_hook_data {
    void *_i2i_entry = nullptr;
    java_hook *hook = nullptr;
};

struct hooked_method {
    hotspot::method *method = nullptr;
    i2i_detour_t detour = nullptr;
};

static std::vector<i2i_hook_data> hooked_i2i_entries{};
static std::vector<hooked_method> hooked_methods{};


void atomic_set_access_flags(hotspot::method *method, jint bits);

void atomic_set_flags(hotspot::method *method, jint bits);


void *find_hook_place(void *i2i_entry);

void common_detour(hotspot::frame *frame, hotspot::thread *thread, bool *cancel);

int create_java_hook(jmethodID methodID, i2i_detour_t detour);

template <typename T>
T get_primitive_param_at(hotspot::frame *frame, const int index) {
    return *(T*)(frame->get_locals() - index);
}

inline jobject get_object_param_at(hotspot::frame *frame, const int index) {
    return (jobject)(frame->get_locals() - index);
}

void clean_java_hooks();


#endif //JAVA_HOOK_H
