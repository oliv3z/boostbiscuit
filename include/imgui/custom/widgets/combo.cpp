
#include "custom/custom.hpp"
#include "ui/gui.h"


using namespace ImGui;


bool widgets::combo(const char *label, int *current_item, const std::vector<const char *> &items) {
    auto *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);

    constexpr float padding = 4.f, gap = 0.f;
    const ImVec2 text_pos = window->DC.CursorPos;

    const ImVec2 pos = text_pos + ImVec2(0.f, GetFrameHeight() + gap);
    const ImVec2 size(250.f, GetFrameHeight() + padding);
    const ImRect bbox(pos, pos + size);

    ItemSize(bbox);
    if (!ItemAdd(bbox, id))
        return false;

    bool hovered, held;
    if (ButtonBehavior(bbox, id, &hovered, &held))  // returns true if pressed
        OpenPopup(id);

    // Label text
    PushFont(GetIO().Fonts->Fonts[1]);
    PushStyleColor(ImGuiCol_Text, gui::sec_text_color);

    ItemSize(CalcTextSize(label));
    RenderText(text_pos, label, nullptr, true);

    PopStyleColor();
    PopFont();

    // Frame
    window->DrawList->AddRectFilled(bbox.Min, bbox.Max, gui::bg_color, 4.f);
    window->DrawList->AddRect(bbox.Min, bbox.Max, gui::border_color, 4.f, 0, 1.f);

    // Value text
    PushFont(GetIO().Fonts->Fonts[1]);
    PushStyleColor(ImGuiCol_Text, gui::sec_text_color);
    RenderTextClipped(bbox.Min + ImVec2(padding, 0), bbox.Max, items[*current_item], nullptr, nullptr, ImVec2(0.f, 0.5f));
    PopStyleColor();
    PopFont();

    // Arrow
    RenderArrow(window->DrawList, bbox.Max - ImVec2(padding + 14.f, size.y / 2.f + 5.f), gui::primary_color, IsPopupOpen(label) ? ImGuiDir_Up : ImGuiDir_Down, 0.75f);

    bool value_changed = false;
    const auto selectable_size = ImVec2(size.x, GetFrameHeight() + 5.f);  // little padding there

    SetNextWindowPos(pos + ImVec2(0.f, size.y), ImGuiCond_Always);
    SetNextWindowSize(ImVec2(size.x, (float)items.size() * selectable_size.y));
    PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.f);
    PushStyleVar(ImGuiStyleVar_PopupRounding, 4.f);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
    PushStyleColor(ImGuiCol_Border, gui::border_color);
    PushStyleColor(ImGuiCol_PopupBg, gui::bg_color);

    if (BeginPopupEx(id, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
        for (int i = 0; i < items.size(); i++) {
            const char* item_text = items[i];

            PushID(i);

            const bool item_selected = i == *current_item;
            if (selectable(item_text, item_selected, selectable_size) && *current_item != i) {
                value_changed = true;
                *current_item = i;
                CloseCurrentPopup();
            }
            if (item_selected)
                SetItemDefaultFocus();

            PopID();
        }

        EndPopup();
    }

    PopStyleColor(2);
    PopStyleVar(4);

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}
