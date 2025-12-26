
#include "java_hook.h"

#include <windows.h>
#include <cstring>

#include "java/jvm/jvm_manager.h"


void* find_hook_place(void *i2i_entry) {
    const uint8_t pattern[] = {
        0x89, 0x84, 0x24, 0x90, 0x90, 0x90, 0x90,
        0x89, 0x84, 0x24, 0x90, 0x90, 0x90, 0x90,
        0x89, 0x84, 0x24, 0x90, 0x90, 0x90, 0x90,
        0x89, 0x84, 0x24, 0x90, 0x90, 0x90, 0x90,
        0x41, 0xC6, 0x87, 0x90, 0x90, 0x90, 0x90, 0x00
    };

    const uint8_t pattern2[] = {
        0x4C, 0x8B, 0x75, 0x90, 0xC3
    };

    const int scan_end = jvm_manager::get().get_client_brand() == ClientBrand::BADLION ? 0x400 : 0x350;  // java 17 is fat

    auto* curr = (uint8_t *)i2i_entry;
    while (curr < (uint8_t *)i2i_entry + scan_end) {
        int matches = 0;
        for (int i = 0; i < sizeof(pattern); ++i) {
            if (pattern[i] == 0x90 || pattern[i] == curr[i])
                matches++;
            else
                break;
        }

        if (matches == sizeof(pattern)) {
            for (uint8_t *curr2 = curr; curr2 > curr - 100; --curr2) {
                int matches2 = 0;
                for (int j = 0; j < sizeof(pattern2); ++j) {
                    if (pattern2[j] == 0x90 || pattern2[j] == curr2[j])
                        matches2++;
                    else
                        break;
                }

                if (matches2 == sizeof(pattern2))  hotspot::frame::locals_offset = *(char *)(curr2 + 3);
            }

            return curr + sizeof(pattern) - 8;
        }

        curr += 1;
    }

    return nullptr;
}


void common_detour(hotspot::frame *frame, hotspot::thread *thread, bool *cancel) {
    if (!*(void **)thread->get_env() || thread->get_thread_state() != hotspot::_thread_in_Java || !jvm_manager::get().get_env())  return;

    for (auto &[method, detour] : hooked_methods) {
        if (method == frame->get_method()) {
            if (detour)  detour(frame, thread, cancel);
            thread->set_thread_state(hotspot::_thread_in_Java);
            return;
        }
    }
}


uint8_t* allocate_nearby_memory(uint8_t *nearby_addr, const int size, const int access) {
    for (int i = 65536; i < 0x7FFFFFFF; i += 65536) {
        auto *allocated = (uint8_t *)VirtualAlloc(nearby_addr + i, size, MEM_COMMIT | MEM_RESERVE, access);
        if (allocated)  return allocated;

        allocated = (uint8_t *)VirtualAlloc(nearby_addr - i, size, MEM_COMMIT | MEM_RESERVE, access);
        if (allocated)  return allocated;
    }

    return nullptr;
}


java_hook::java_hook(uint8_t *target, i2i_detour_t detour) : target(target) {
    constexpr int HOOK_SIZE = 8;
    constexpr int JMP_SIZE = 5;
    constexpr int JE_OFFSET = 0x3d;
    constexpr int JE_SIZE = 6;
    constexpr int DETOUR_ADDRESS_OFFSET = 0x56;

    uint8_t assembly[] = {
        0x50, 0x51, 0x52, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0x55, 0x6A, 0x00, 0x48, 0x89, 0xE9, 0x4C, 0x89, 0xFA, 0x4C, 0x8D, 0x04,
        0x24, 0x48, 0x89, 0xE5, 0x48, 0x83, 0xE4, 0xF0, 0x48, 0x83, 0xEC, 0x20, 0xFF, 0x15, 0x2D, 0x00, 0x00, 0x00, 0x48, 0x89, 0xEC, 0x58, 0x48,
        0x83, 0xF8, 0x00, 0x5D, 0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5A, 0x59, 0x58, 0x0F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x66, 0x48,
        0x0F, 0x6E, 0xC0, 0x48, 0x8B, 0x5D, 0xF8, 0x48, 0x89, 0xEC, 0x5D, 0x5E, 0x48, 0x89, 0xDC, 0xFF, 0xE6,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // detour address here
    };

    allocated_assembly = allocate_nearby_memory(target, HOOK_SIZE + sizeof(assembly), PAGE_EXECUTE_READWRITE);
    if (!allocated_assembly) {
        logger::get().error("Failed to allocate memory for i2i hook");
        return;
    }

    const auto jmp_back_delta = (int32_t)(target + HOOK_SIZE - (allocated_assembly + HOOK_SIZE + JE_OFFSET + JE_SIZE));
    *(int32_t *)(assembly + JE_OFFSET + 2) = jmp_back_delta;

    *(i2i_detour_t *)(assembly + DETOUR_ADDRESS_OFFSET) = detour;

    memcpy(allocated_assembly, target, HOOK_SIZE);
    memcpy(allocated_assembly + HOOK_SIZE, assembly, sizeof(assembly));

    DWORD original_prot = 0;

    VirtualProtect(allocated_assembly, HOOK_SIZE + sizeof(assembly), PAGE_EXECUTE_READ, &original_prot);
    VirtualProtect(target, JMP_SIZE, PAGE_EXECUTE_READWRITE, &original_prot);

    target[0] = 0xE9U;

    const auto jmp_detour_delta = (int32_t)(allocated_assembly - (target + JMP_SIZE));
    *(int32_t *)(target + 1) = jmp_detour_delta;

    VirtualProtect(target, JMP_SIZE, original_prot, &original_prot);

    is_error = false;
}


java_hook::~java_hook() {
    if (is_error)  return;

    DWORD original = 0;
    if (target[0] == 0xE9U && VirtualProtect(target, 5, PAGE_EXECUTE_READWRITE, &original)) {
        memcpy(target, allocated_assembly, 5);
        VirtualProtect(target, 5, original, &original);
    }

    VirtualFree(allocated_assembly, 0, MEM_RELEASE);
}


void atomic_set_access_flags(hotspot::method *method, const jint bits) {
    const jint old_flags = *(jint *)(method->get_access_flags());
    const jint new_flags = old_flags | bits;

    *(jint *)method->get_access_flags() = new_flags;
}


void atomic_set_flags(hotspot::method *method, const jint bits) {
    const jint old_flags = *(jint *)method->get_flags();
    const jint new_flags = old_flags | bits;

    *(jint *)method->get_flags() = new_flags;
}


void clean_java_hooks() {
    logger::get().info("Cleaning hooked I2I entries...");
    for (auto &[_i2i_entry, hook] : hooked_i2i_entries) {
        if (hook) {
            delete hook;
            hook = nullptr;
        }
    }

    logger::get().info("Cleaning hooked methods...");
    for (auto &[method, detour] : hooked_methods) {
        if (method) {
            method->set_inlining(true);
            auto* accessFlags = (int *)method->get_access_flags();
            *accessFlags &= ~hotspot::NO_COMPILE;
        }
    }

    hooked_i2i_entries.clear();
    hooked_methods.clear();
}


int create_java_hook(jmethodID methodID, const i2i_detour_t detour) {
    if (!methodID || !jvm_manager::get().get_env() || !detour) {
        logger::get().error("Invalid methodID or Java environment or detour");
        return 1;
    }

    hotspot::method* method = *(hotspot::method **)methodID;

    method->set_inlining(false);

    atomic_set_access_flags(method, hotspot::JVM_ACC_NOT_C1_COMPILABLE);
    atomic_set_access_flags(method, hotspot::JVM_ACC_NOT_C2_COMPILABLE);
    atomic_set_access_flags(method, hotspot::JVM_ACC_NOT_C2_OSR_COMPILABLE);
    atomic_set_access_flags(method, hotspot::JVM_ACC_QUEUED);

    jclass owner = nullptr;
    jvm_manager::get().get_jvmti_env()->GetMethodDeclaringClass(methodID, &owner);
    jvm_manager::get().get_jvmti_env()->RetransformClasses(1, &owner);
    jvm_manager::get().get_env()->DeleteLocalRef(owner);

    method = *(hotspot::method **)methodID;
    if (!method) {
        logger::get().error("Failed to get method");
        return 1;
    }

    method->set_inlining(false);

    atomic_set_access_flags(method, hotspot::JVM_ACC_NOT_C1_COMPILABLE);
    atomic_set_access_flags(method, hotspot::JVM_ACC_NOT_C2_COMPILABLE);
    atomic_set_access_flags(method, hotspot::JVM_ACC_NOT_C2_OSR_COMPILABLE);
    atomic_set_access_flags(method, hotspot::JVM_ACC_QUEUED);

    hooked_methods.push_back({ method, detour });

    void* i2i_entry = method->get_i2i_entry();
    if (!i2i_entry) {
        logger::get().error("Failed to get i2i entry");
        return 1;
    }

    for (const auto &h_i2i_entry : hooked_i2i_entries) {
        if (h_i2i_entry._i2i_entry == i2i_entry) {
            return 0;
        }
    }

    auto* target = (uint8_t *)find_hook_place(i2i_entry);
    if (!target) {
        logger::get().error("Failed to find hook place");
        return 1;
    }

    auto *hook = new java_hook(target, common_detour);  // does not leak if clean_hooks() is called

    hooked_i2i_entries.push_back({ i2i_entry, hook });

    return 0;
}

// TODO: implement RAII for hooked_i2i_entries and hooked_methods for safer cleanup
