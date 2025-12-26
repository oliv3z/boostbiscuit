
#include "custom/custom.hpp"
#include "ui/gui.h"


using namespace ImGui;


bool widgets::off_on_button(const char *id_str, bool *value, const ImVec2 &pos) {
    auto *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const auto id = window->GetID(id_str);

    const auto button_size = ImVec2(50.f, 15.f);
    const auto bbox = ImRect(pos, pos + button_size);

    // NOTE: not adding item size here because we use draw list directly
    if (!ItemAdd(bbox, id))
        return false;

    bool hovered, held;
    const bool pressed = ButtonBehavior(bbox, id, &hovered, &held);
    if (pressed)
        *value = !*value;

    // render frame
    const ImU32 bg_color = *value ? gui::primary_color : gui::bg_color;
    window->DrawList->AddRectFilled(bbox.Min, bbox.Max, bg_color, 4.f);

    // render text
    const ImU32 text_color = *value ? gui::prim_text_color : gui::sec_text_color;
    PushFont(GetIO().Fonts->Fonts[0]);
    const auto text_size = CalcTextSize(*value ? "ON" : "OFF", nullptr, true);
    window->DrawList->AddText(bbox.GetCenter() - text_size / 2.f, text_color, *value ? "ON" : "OFF");
    PopFont();

    return pressed;
}
