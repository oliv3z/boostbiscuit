
#ifndef BOOSTBISCUIT_H
#define BOOSTBISCUIT_H

#include <atomic>
#include <string>
#include <windows.h>

struct Process {
    DWORD pid;
    HWND window;
};

namespace bb {
    constexpr std::string version = "0.1.0";
    constexpr bool debug_mode = true;

    inline HMODULE h_module = nullptr;
    inline HWND h_window = nullptr;
    inline std::atomic running = true;
}

int __stdcall MainThread(LPVOID lpParam);


#endif //BOOSTBISCUIT_H
