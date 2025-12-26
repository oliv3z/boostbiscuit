
#include "custom/custom.hpp"
#include "ui/gui.h"


using namespace ImGui;


bool widgets::text_button(const char *label, int *active_button_index, const ImVec2 &size, const int button_index) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);

    // Define bounding box at current cursor
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect bbox(pos, pos + size);

    // Advance layout & register item
    ItemSize(bbox);
    if (!ItemAdd(bbox, id))
        return false;

    // Input handling
    bool hovered, held;
    const bool pressed = ButtonBehavior(bbox, id, &hovered, &held);

    if (pressed && active_button_index)
        *active_button_index = button_index;

    const bool is_active = active_button_index && *active_button_index == button_index;

    // background (border)
    // window->DrawList->AddRectFilled(bbox.Min + ImVec2(1.f, 1.f), bbox.Max - ImVec2(1.f, 1.f), gui::bg_color, 4.f);
    // window->DrawList->AddRect(bbox.Min, bbox.Max, is_active ? gui::prim_text_color : gui::border_color, 4.f, 0, 1.f);

    // Label (centered)
    PushFont(g.IO.Fonts->Fonts[1]);

    const ImVec2 text_size = CalcTextSize(label, nullptr, true);
    const ImVec2 text_pos = bbox.GetCenter() - text_size * 0.5f;

    window->DrawList->AddText(text_pos, is_active ? gui::primary_color : gui::sec_text_color, label);

    PopFont();

    return pressed;
}
