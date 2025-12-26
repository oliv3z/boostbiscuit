
#include "custom/custom.hpp"
#include "ui/gui.h"

using namespace ImGui;


bool widgets::checkbox(const char *label, bool *value, float width) {
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiID id = window->GetID(label);

    constexpr float box_size = 17.f;
    // constexpr float gap = 5.f;  // useless when width is set
    const ImVec2 cursor_pos = window->DC.CursorPos;
    const ImVec2 label_size = CalcTextSize(label, nullptr, true);
    const ImRect bbox(ImVec2(cursor_pos.x + width - box_size, cursor_pos.y), ImVec2(cursor_pos.x + width, cursor_pos.y + box_size));

    ItemSize(bbox, style.FramePadding.y);
    if (!ItemAdd(bbox, id))
        return false;

    bool hovered, held;
    const bool pressed = ButtonBehavior(bbox, id, &hovered, &held);
    if (pressed) {
        *value = !*value;
        MarkItemEdited(id);
    }

    // render label
    PushFont(GetIO().Fonts->Fonts[1]);

    const auto label_pos = ImVec2(cursor_pos.x, bbox.GetCenter().y - label_size.y / 2.f);
    if (label_size.x > 0.f)
        window->DrawList->AddText(label_pos, gui::sec_text_color, label);

    PopFont();

    // render frame
    window->DrawList->AddRectFilled(bbox.Min + ImVec2(1, 1), bbox.Max - ImVec2(1, 1), *value ? gui::primary_color : gui::bg_color, 2.f);
    window->DrawList->AddRect(bbox.Min, bbox.Max, gui::border_color, 2.f, 0, 1.f);

    return pressed;
}
