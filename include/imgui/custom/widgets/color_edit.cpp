
#include "custom/custom.hpp"
#include "ui/gui.h"


using namespace ImGui;


bool widgets::color_edit(const char *label, float *col, const float width, bool *enabled) {
    const auto window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    constexpr float preview_box_size = 23.f, gap = 10.f, cb_size = 17.f;

    PushID(label);

    const auto cursor_pos = window->DC.CursorPos;

    const auto id = window->GetID(label);
    const auto label_size = CalcTextSize(label, nullptr, true);

    // render text
    PushFont(GetIO().Fonts->Fonts[1]);
    window->DrawList->AddText(cursor_pos + ImVec2(0.f, (preview_box_size - label_size.y) / 2.f), gui::sec_text_color, label);
    PopFont();

    // state is toggleable
    if (enabled != nullptr) {
        const auto cb_id = window->GetID("##checkbox");
        const auto cb_bbox = ImRect(ImVec2(cursor_pos.x + width - cb_size, cursor_pos.y + (preview_box_size - cb_size) / 2.f),
            ImVec2(cursor_pos.x + width, cursor_pos.y + (preview_box_size + cb_size) / 2.f));

        if (!ItemAdd(cb_bbox, cb_id)) {
            PopID();
            return false;
        }

        bool hovered, held;
        const bool pressed = ButtonBehavior(cb_bbox, cb_id, &hovered, &held);
        if (pressed) {
            *enabled = !*enabled;
            MarkItemEdited(cb_id);
        }

        // render checkbox
        window->DrawList->AddRectFilled(cb_bbox.Min + ImVec2(1, 1), cb_bbox.Max - ImVec2(1, 1), *enabled ? gui::primary_color : gui::bg_color, 2.f);
        window->DrawList->AddRect(cb_bbox.Min, cb_bbox.Max, gui::border_color, 2.f, 0, 1.f);
    }

    ImU32 preview_col = IM_COL32((ImU8)(col[0] * 255.f), (ImU8)(col[1] * 255.f), (ImU8)(col[2] * 255.f), (ImU8)(col[3] * 255.f));
    const auto preview_bbox = ImRect(ImVec2(cursor_pos.x + width - preview_box_size - (enabled != nullptr ? (cb_size + gap) : 0.f),
        cursor_pos.y), ImVec2(cursor_pos.x + width - (enabled != nullptr ? (cb_size + gap) : 0.f), cursor_pos.y + preview_box_size));
    const auto preview_id = window->GetID("##colorpreview");

    if (!ItemAdd(preview_bbox, preview_id)) {
        PopID();
        return false;
    }

    bool hovered, held;
    if (ButtonBehavior(preview_bbox, preview_id, &hovered, &held)) {
        OpenPopup(id);
    }

    // render preview box
    window->DrawList->AddRectFilled(preview_bbox.Min + ImVec2(1, 1), preview_bbox.Max - ImVec2(1, 1), preview_col, 4.f);
    window->DrawList->AddRect(preview_bbox.Min, preview_bbox.Max, gui::border_color, 4.f, 0, 1.f);

    SetNextWindowPos(preview_bbox.Min + ImVec2(0.f, preview_box_size + 5.f), ImGuiCond_Always);
    PushStyleVar(ImGuiStyleVar_PopupRounding, 5.f);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
    PushStyleColor(ImGuiCol_PopupBg, gui::bg_color);
    PushStyleColor(ImGuiCol_Border, gui::border_color);

    if (BeginPopupEx(id, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
        PushStyleColor(ImGuiCol_FrameBg, gui::bg_color);
        PushStyleColor(ImGuiCol_FrameBgActive, gui::hover_color);
        PushStyleColor(ImGuiCol_FrameBgHovered, gui::hover_color);

        ColorPicker4("ESP color", col,
            ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoLabel);

        PopStyleColor(3);

        EndPopup();
    }

    PopStyleColor(2);
    PopStyleVar(3);

    PopID();

    // one for all
    ItemSize(ImVec2(width, preview_box_size));

    return true;
}
