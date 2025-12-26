
#ifndef CUSTOM_H
#define CUSTOM_H

#define IMGUI_DEFINE_MATH_OPERATORS

#include <vector>
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

// ty raid0 heh!


namespace widgets {
    bool text_button(const char *label, int *active_button_index, const ImVec2 &size, int button_index);
    bool navbar(const std::vector<const char *> &section_labels, const ImVec2 &size, int *active_section_idx);
    bool begin_child(const char *name, float width, float px = 5.f);  // px = x padding
    bool end_child(const char *name, int *p_keybind, bool *p_enabled);  // also needs name for title text
    bool off_on_button(const char *id_str, bool *value, const ImVec2 &pos);
    bool keybind(const char *id_str, int *keybind_ptr, const ImVec2 &pos);

    IMGUI_API bool checkbox(const char *label, bool *v, float width);
    IMGUI_API bool combo(const char *label, int *current_item, const std::vector<const char *> &items);
    IMGUI_API bool selectable(const char *label, bool is_selected, const ImVec2 &size);
    IMGUI_API bool color_edit(const char *label, float *col, float width, bool *enabled = nullptr);
    IMGUI_API bool slider(const char *label, ImGuiDataType data_type, void *v, const void *v_min, const void *v_max, const char *format, float width);
}


#endif //CUSTOM_H
