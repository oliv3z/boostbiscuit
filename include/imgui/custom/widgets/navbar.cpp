
#include "custom/custom.hpp"


using namespace ImGui;


bool widgets::navbar(const std::vector<const char *> &section_labels, const ImVec2 &size, int *active_section_idx) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems || section_labels.empty())
        return false;

    const float w = size.x / (float)section_labels.size();
    bool changed = false;

    for (int i = 0; i < (int)section_labels.size(); ++i) {
        if (i > 0)
            SameLine(0.0f, 0.0f);

        PushID(i);
        if (text_button(section_labels[i], active_section_idx, ImVec2(w, size.y), i))
            changed = true;
        PopID();
    }

    return changed;
}
