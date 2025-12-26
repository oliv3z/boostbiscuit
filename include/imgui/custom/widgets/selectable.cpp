
#include "custom/custom.hpp"
#include "ui/gui.h"


using namespace ImGui;


bool widgets::selectable(const char *label, bool is_selected, const ImVec2 &size) {
    auto *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const auto id = window->GetID(label);
    const auto cursor_pos = window->DC.CursorPos;

    PushFont(GetIO().Fonts->Fonts[0]);

    const auto label_size = CalcTextSize(label, nullptr, true);
    const auto bbox = ImRect(cursor_pos, cursor_pos + size);

    PopFont();

    ItemSize(bbox);
    if (!ItemAdd(bbox, id))
        return false;

    bool hovered, held;
    const bool pressed = ButtonBehavior(bbox, id, &hovered, &held);
    const ImU32 bg_color = !is_selected ? (hovered ? gui::hover_color : gui::bg_color) : gui::dim_primary_color;

    // render it !
    window->DrawList->AddRectFilled(bbox.Min, bbox.Max, bg_color, 8.f);
    window->DrawList->AddText(bbox.GetCenter() - label_size / 2.f, gui::sec_text_color, label);

    return pressed;
}
