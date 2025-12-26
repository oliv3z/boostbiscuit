
#include "custom/custom.hpp"

#include <windows.h>

#include "ui/gui.h"
#include "utils/logs/logger.h"


using namespace ImGui;


const char* key_to_string(const int vk) {
    switch (vk) {
        case VK_LBUTTON: return "Mouse 1";
        case VK_RBUTTON: return "Mouse 2";
        case VK_MBUTTON: return "Mouse 3";
        case VK_XBUTTON1: return "Mouse x1";
        case VK_XBUTTON2: return "Mouse x2";
        case VK_SHIFT: return "Shift";
        case VK_CONTROL: return "Ctrl";
        case VK_MENU: return "Alt";
        case VK_TAB: return "Tab";
        case VK_SPACE: return "Space";
        case VK_RETURN: return "Enter";
        case VK_ESCAPE: return "Esc";
        default:
            static char buf[32];
            if (GetKeyNameTextA(MapVirtualKeyA(vk, MAPVK_VK_TO_VSC) << 16, buf, sizeof(buf)))
                return buf;
            return "Unknown";
    }
}


void WndProc_callback(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param, int *keybind_ptr) {
    if (msg == WM_KEYDOWN) {
        if (w_param == VK_ESCAPE) {
            *keybind_ptr = 0;  // cancel keybind
            return;
        }

        *keybind_ptr = static_cast<int>(w_param);
    } else if (msg == WM_LBUTTONDOWN) {
        *keybind_ptr = VK_LBUTTON;
    } else if (msg == WM_RBUTTONDOWN) {
        *keybind_ptr = VK_RBUTTON;
    } else if (msg == WM_XBUTTONDOWN) {
        if (HIWORD(w_param) == XBUTTON1)
            *keybind_ptr = VK_XBUTTON1;
        else if (HIWORD(w_param) == XBUTTON2)
            *keybind_ptr = VK_XBUTTON2;
    }
}


// pressing escape will remove keybind keep in mind!
bool widgets::keybind(const char *id_str, int *keybind_ptr, const ImVec2 &pos) {
    auto *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const auto id = window->GetID(id_str);

    PushFont(GetIO().Fonts->Fonts[0]);

    constexpr float px = 4.f;
    const auto label = *keybind_ptr != 0 ? key_to_string(*keybind_ptr) : (gui::keybind_ptr != nullptr ? "..." : "_");
    const auto label_size = CalcTextSize(label);
    const auto size = ImVec2(label_size.x + px * 2, 15.f);
    const auto bbox = ImRect(pos, pos + size);

    // NOTE: not adding item size here because we use draw list directly
    if (!ItemAdd(bbox, id))
        return false;

    bool hovered, held;
    const bool pressed = ButtonBehavior(bbox, id, &hovered, &held);
    if (pressed) {
        gui::WndProc_callback = WndProc_callback;
        *keybind_ptr = 0;
        gui::keybind_ptr = keybind_ptr;
    }

    // render text
    window->DrawList->AddText(bbox.GetCenter() - label_size / 2.f, gui::sec_text_color, label);

    PopFont();

    // render frame
    window->DrawList->AddRect(bbox.Min, bbox.Max, gui::border_color, 4.f);

    return true;
}
