
#include "custom/custom.hpp"
#include "ui/gui.h"


using namespace ImGui;


bool widgets::slider(const char *label, ImGuiDataType data_type, void *v, const void *v_min, const void *v_max, const char *format, const float width) {
    auto *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const auto cursor_pos = GetCursorScreenPos();
    const auto id = window->GetID(label);

    char v_label[64];
    float t = 0.f;

    switch (data_type) {
        case ImGuiDataType_Float:
            t = (*(float*)v - *(float*)v_min) / (*(float*)v_max - *(float*)v_min);
            snprintf(v_label, sizeof(v_label), "%.2f blocks", *(float*)v);
            break;
        case ImGuiDataType_Double:
            t = (*(double*)v - *(double*)v_min) / (*(double*)v_max - *(double*)v_min);
            snprintf(v_label, sizeof(v_label), "%.2f blocks", *(double*)v);
            break;
        case ImGuiDataType_S32:
            t = float(*(int*)v - *(int*)v_min) / float(*(int*)v_max - *(int*)v_min);
            snprintf(v_label, sizeof(v_label), "%d blocks", *(int*)v);
            break;
        case ImGuiDataType_U32:
            t = float(*(unsigned int*)v - *(unsigned int*)v_min) / float(*(unsigned int*)v_max - *(unsigned int*)v_min);
            snprintf(v_label, sizeof(v_label), "%u blocks", *(unsigned int*)v);
            break;
        default:
            t = 0.f;
            snprintf(v_label, sizeof(v_label), "? blocks");
            break;
    }

    PushFont(GetIO().Fonts->Fonts[1]);

    const auto label_pos = ImVec2(cursor_pos.x, cursor_pos.y);
    const auto v_label_size = CalcTextSize(v_label);
    const auto v_label_pos = ImVec2(cursor_pos.x + width - v_label_size.x, cursor_pos.y);
    constexpr float py = 5.f, slider_height = 10.f;
    auto bbox = ImRect(
        ImVec2(cursor_pos.x, cursor_pos.y + py + v_label_size.y),
        ImVec2(cursor_pos.x + width, cursor_pos.y + py + v_label_size.y + slider_height)
    );

    // render the thing
    ItemSize(bbox);
    if (!ItemAdd(bbox, id)) {
        PopFont();
        return false;
    }

    // for the text section
    ItemSize(v_label_size + ImVec2(0.f, py));

    bool hovered = false, held = false;
    ButtonBehavior(bbox, id, &hovered, &held, ImGuiButtonFlags_None);

    ImRect grab_bb;
    const bool changed = SliderBehavior(bbox, id, data_type, v, v_min, v_max, format, ImGuiSliderFlags_None, &grab_bb);

    // render label and value label
    window->DrawList->AddText(label_pos, gui::sec_text_color, label, nullptr);
    window->DrawList->AddText(v_label_pos, gui::sec_text_color, v_label, nullptr);

    // slider background
    window->DrawList->AddRectFilled(bbox.Min, bbox.Max, gui::dim_border_color, 4.f);

    // slider fill (from left to grab)
    if (grab_bb.Max.x > grab_bb.Min.x) {
        t = ImClamp(t, 0.0f, 1.0f);
        window->DrawList->AddRectFilled(bbox.Min, ImVec2(ImLerp(bbox.Min.x, bbox.Max.x, t), bbox.Max.y), gui::primary_color, 4.f);
    }

    PopFont();

    return changed;
}
