
#include "custom/custom.hpp"
#include "ui/gui.h"


using namespace ImGui;


bool widgets::begin_child(const char *name, const float width, const float px) {
    const auto *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    BeginChild(name, ImVec2(width, 0), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(px, 30));  // padding + account for title

    return true;
}


bool widgets::end_child(const char *name, int *p_keybind, bool *p_enabled) {
    const ImGuiWindow* window = GetCurrentWindow();
    ImDrawList* draw_list = GetWindowDrawList();

    const ImVec2 win_min = window->Pos;
    const ImVec2 win_size = window->Size;
    const ImVec2 win_max = win_min + win_size;
    constexpr float rounding = 10.0f;

    // draw borders around child
    draw_list->AddRect(win_min, win_max, gui::dim_border_color, rounding, ImDrawFlags_RoundCornersAll, 1.0f);

    // draw header
    PushFont(GetIO().Fonts->Fonts[2]);
    constexpr float header_py = 5.f, header_px = 10.f;
    const auto text_size = CalcTextSize(name);
    draw_list->AddText(ImVec2(win_min.x + header_px, win_min.y + header_py), gui::prim_text_color, name);
    PopFont();

    const auto keybind_pos = ImVec2(win_min.x + text_size.x + header_px * 2, win_min.y + 5.f);  // 110 is button width + some spacing
    keybind((std::string(name) + "_keybind").c_str(), p_keybind, keybind_pos);

    const auto toggle_pos = ImVec2(win_max.x - header_px - 50.f, win_min.y + 5.f);  // 50 is button width
    off_on_button((std::string(name) + "_enabled").c_str(), p_enabled, toggle_pos);

    // draw line under header
    draw_list->AddLine(ImVec2(win_min.x, win_min.y + text_size.y + header_py * 2), ImVec2(win_max.x, win_min.y + text_size.y + header_py * 2), gui::dim_border_color, 1.0f);

    EndChild();

    return true;
}
