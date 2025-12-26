
#include "boostbiscuit.h"

#include <thread>

#include "mapping/mapping.h"
#include "utils/logs/logger.h"
#include "hooks/hook_manager.h"
#include "java/jvm/jvm_manager.h"
#include "features/module/module_manager.h"
#include "features/module/other/render_data.h"
#include "wrapper/minecraft/client/Minecraft.h"


void DetachEarly(const int exitCode) {
    bb::h_window = nullptr;

    logger::get().info("Detaching from process...");

    FreeLibraryAndExitThread(bb::h_module, exitCode);
}


static BOOL CALLBACK EnumWindowsCallback(_In_ HWND hwnd, _In_ LPARAM lParam) {
    auto* pProcess = reinterpret_cast<Process*>(lParam);

    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);

    if (pid == pProcess->pid && GetWindow(hwnd, GW_OWNER) == nullptr && IsWindowVisible(hwnd) && GetConsoleWindow() != hwnd) {
        pProcess->window = hwnd;
        return FALSE;
    }

    return TRUE;
}


HWND getCurrentWindow() {
    HANDLE hProcess = GetCurrentProcess();
    Process process = { .pid = GetProcessId(hProcess) };

    EnumWindows(&EnumWindowsCallback, reinterpret_cast<LPARAM>(&process));
    return process.window;
}


/*
void find_fields() {
    jvmtiEnv* jvmti = jvm_manager::get().get_jvmti_env();

    jint class_count;
    jclass* classes;

    // Get all loaded classes
    jvmtiError err = jvmti->GetLoadedClasses(&class_count, &classes);
    if (err != JVMTI_ERROR_NONE) {
        logger::get().error("Failed to get loaded classes: %d " + err);
        return;
    }

    for (int i = 0; i < class_count; i++) {
        char* class_sig = nullptr;
        err = jvmti->GetClassSignature(classes[i], &class_sig, nullptr);
        if (err != JVMTI_ERROR_NONE) continue;

        if (strcmp(class_sig, "Lnet/badlion/clientcommon/util/aF;") != 0) {
            jvmti->Deallocate((unsigned char*)class_sig);
            continue;  // only interested in this class for now
        }

        jint field_count;
        jfieldID* fields;

        err = jvmti->GetClassFields(classes[i], &field_count, &fields);
        if (err != JVMTI_ERROR_NONE) {
            jvmti->Deallocate((unsigned char*)class_sig);
            continue;
        }

        for (int f = 0; f < field_count; f++) {
            char* name = nullptr;
            char* signature = nullptr;
            char* generic = nullptr;

            err = jvmti->GetFieldName(classes[i], fields[f], &name, &signature, &generic);
            if (err != JVMTI_ERROR_NONE) continue;

            jint modifiers;
            jvmti->GetFieldModifiers(classes[i], fields[f], &modifiers);

            // 0x0008 is the static modifier
            if (!(modifiers & 0x0008)) {
                if (name) jvmti->Deallocate((unsigned char*)name);
                if (signature) jvmti->Deallocate((unsigned char*)signature);
                if (generic) jvmti->Deallocate((unsigned char*)generic);
                continue;  // skip non-static fields
            }

            // Check if field is FloatBuffer
            //if (signature && strcmp(signature, "Ljava/nio/FloatBuffer;") == 0) {
                logger::get().info("Class " + std::string(class_sig) + " has field: " + name + " with signature: " + signature);
            //}

            if (name) jvmti->Deallocate((unsigned char*)name);
            if (signature) jvmti->Deallocate((unsigned char*)signature);
            if (generic) jvmti->Deallocate((unsigned char*)generic);
        }

        if (class_sig) jvmti->Deallocate((unsigned char*)class_sig);
    }

    if (classes) jvmti->Deallocate((unsigned char*)classes);
}
*/


int __stdcall MainThread(LPVOID lpParam) {
    // INIT START

    bb::h_module = static_cast<HMODULE>(lpParam);
    bb::h_window = getCurrentWindow();

    logger::get().info("BoostBiscuit v" + bb::version + " attached to process!");

    if (!jvm_manager::get().initialize()) {
        logger::get().error("Failed to initialize JVM Manager, exiting main thread...");
        bb::running.store(false);
        return 1;
    }
    logger::get().info("JVM Manager initialized!");

    if (!mapping::initialize()) {
        logger::get().error("Failed to initialize mapper, exiting main thread...");
        bb::running.store(false);
        return 1;
    }
    logger::get().info("Mapper initialized!");

    hooks::hook_manager::init();
    hooks::hook_manager::get().setup_hooks();
    logger::get().info("Hooks initialized!");

    module_manager::get().init();
    module_manager::get().run_all_modules();
    module_manager::get().enable_defaults();  // temp
    logger::get().info("Modules initialized!");
    // INIT END

    while (bb::running.load()) {
        if (GetAsyncKeyState(VK_END) & 1)
            bb::running.store(false);
        Sleep(10);
    }

    // CLEANUP START
    hooks::hook_manager::get().cleanup();
    // CLEANUP END

    // DETACH SAFELY
    CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
        Sleep(1000);  // Ensure cleanup is successful
        FreeLibraryAndExitThread(bb::h_module, 0);
    }, nullptr, 0, nullptr);

    Sleep(500);

    logger::get().info("Detaching from process...");

    return 0;  // stack unwinding here
}
