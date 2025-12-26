
#include "hotspot.h"

#include <cstring>

#include "utils/logs/logger.h"


extern "C" JNIIMPORT hotspot::vm_struct_entry *gHotSpotVMStructs;


bool hotspot::init() {
    return !!gHotSpotVMStructs;
}


hotspot::vm_struct_entry* hotspot::find_vm_struct_entry(const char* type_name, const char* field_name, const bool is_static) {
    for (vm_struct_entry* entry = gHotSpotVMStructs; entry->type_name != nullptr; ++entry) {
        if (type_name && std::strcmp(type_name, entry->type_name) != 0)  continue;
        if (field_name && std::strcmp(field_name, entry->field_name) != 0)  continue;
        if (is_static != (bool)entry->is_static) continue;

        return entry;
    }

    return nullptr;
}


void hotspot::dump_vm_struct_entries() {  // for d bugging
    for (const vm_struct_entry* vm_struct_entry = gHotSpotVMStructs; vm_struct_entry->type_name != nullptr; ++vm_struct_entry) {
        logger::get().info(
            "Type: " + std::string(vm_struct_entry->type_name) +
            ", Field: " + std::string(vm_struct_entry->field_name) +
            ", Offset: " + std::to_string(vm_struct_entry->offset) +
            ", IsStatic: " + std::to_string(vm_struct_entry->is_static)
        );
    }
}


unsigned short* hotspot::method::get_flags() {
    static vm_struct_entry* flagsEntry = find_vm_struct_entry("Method", "_flags", false);
    if (flagsEntry == nullptr)  return nullptr;

    return reinterpret_cast<unsigned short*>(reinterpret_cast<uint8_t*>(this) + flagsEntry->offset);
}


hotspot::access_flags* hotspot::method::get_access_flags() {
    static vm_struct_entry* accessFlagsEntry = find_vm_struct_entry("Method", "_access_flags", false);
    if (!accessFlagsEntry) {
        logger::get().error("access flags entry not found");
        return nullptr;
    }

    return (access_flags *)(reinterpret_cast<uint8_t*>(this) + accessFlagsEntry->offset);
}


void hotspot::method::set_inlining(const bool enabled) {
    unsigned short* flags = get_flags();

    if (!flags) {
        static vm_struct_entry* methodEntry = find_vm_struct_entry("Method", "_intrinsic_id", false);
        if (methodEntry == nullptr) return;

        auto* _flags = (uint8_t *)(this + methodEntry->offset + 1);
        if (enabled) {
            *_flags &= ~(1 << 4);
        } else {
            *_flags |= (1 << 4);
        }

        return;
    }

    if (enabled)  *flags &= ~(1 << 2);
    else  *flags |= (1 << 2);
}


void *hotspot::method::get_i2i_entry() {
    static vm_struct_entry* i2iEntry = find_vm_struct_entry("Method", "_i2i_entry", false);
    if (i2iEntry == nullptr)  return nullptr;

    return *(void **)((uint8_t *)this + i2iEntry->offset);
}


JNIEnv* hotspot::thread::get_env() {
    static vm_struct_entry* envEntry = find_vm_struct_entry("JavaThread", "_anchor", false);
    if (envEntry == nullptr)  return nullptr;

    return (JNIEnv *)((uint8_t *)this + envEntry->offset + 32);
}


int hotspot::thread::get_thread_state_offset() {
    static vm_struct_entry* stateEntry = find_vm_struct_entry("JavaThread", "_thread_state", false);
    if (!stateEntry)  return 0;

    return (int)stateEntry->offset;
}


hotspot::java_thread_state hotspot::thread::get_thread_state() {
    return *(java_thread_state *)((uint8_t *)this + get_thread_state_offset());
}


void hotspot::thread::set_thread_state(java_thread_state state) {
    *(java_thread_state *)((uint8_t *)this + get_thread_state_offset()) = state;
}


hotspot::method *hotspot::frame::get_method() {
    return *(method **)((uint8_t *)this - 24);
}


void** hotspot::frame::get_locals() {
    return *(void ***)((uint8_t *)this + locals_offset);
}
