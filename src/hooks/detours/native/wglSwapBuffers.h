
#ifndef WGLSWAPBUFFERS_H
#define WGLSWAPBUFFERS_H

#include <imgui.h>
#include <windows.h>


class wglSwapBuffers {
public:
    static inline BOOL(WINAPI *wglSwapBuffers_original)(HDC);
    static inline LONG_PTR WndProc_original = 0;

    static BOOL __stdcall wglSwapBuffers_detour(HDC hdc);
    static LRESULT CALLBACK WndProc_detour(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif //WGLSWAPBUFFERS_H
