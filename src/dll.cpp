
#include "boostbiscuit.h"

#include <windows.h>
#include <iostream>


BOOL APIENTRY DllMain(HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);  // Prevent DLL_THREAD_ATTACH and DLL_THREAD_DETACH from being called

            CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), hModule, 0, nullptr);

            break;
        case DLL_PROCESS_DETACH:
            break;
        default:
            break;
    }

    return TRUE;
}
