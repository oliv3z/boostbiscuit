
#ifndef GUI_H
#define GUI_H

#define IMGUI_DEFINE_MATH_OPERATORS

#include <GL/gl.h>
#include <imgui.h>
#include <jni.h>


class gui {
    const int gui_bind = VK_INSERT;
    bool is_first_frame = true;
    bool is_being_drawn = false;
    ImGuiContext *imgui_ctx = nullptr;
    HDC device = nullptr;

    static void render_title_bar();

    static void render_render_category();

    void render_menu(const GLint viewport[4]);

public:
    static constexpr ImU32 prim_text_color =    IM_COL32(238, 238, 238, 255);
    static constexpr ImU32 sec_text_color =     IM_COL32(153, 153, 153, 255);
    static constexpr ImU32 border_color =       IM_COL32(30, 30, 30, 255);
    static constexpr ImU32 dim_border_color =   IM_COL32(18, 18, 18, 255);
    static constexpr ImU32 bg_color =           IM_COL32(9, 9, 9, 255);
    static constexpr ImU32 hover_color =        IM_COL32(27, 27, 27, 255);
    static constexpr ImU32 primary_color =      IM_COL32(238, 30, 44, 255);  // cherry red
    static constexpr ImU32 dim_primary_color =  IM_COL32(238, 30, 44, 127);

    static inline void(*WndProc_callback)(HWND, UINT, WPARAM, LPARAM, int*) = nullptr;
    static inline int *keybind_ptr = nullptr;

    static gui& get() {
        static gui instance;
        return instance;
    }

    void init(ImGuiContext *ctx, HDC device_, HWND h_window);

    void destroy();

    void render(const GLint viewport[4]);

    bool WndProc_handler(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param);
};


#endif //GUI_H
