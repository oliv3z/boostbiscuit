
#include "gui.h"

#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

#include "boostbiscuit.h"
#include "custom/custom.hpp"
#include "fonts/GeistMedium.h"
#include "java/jvm/jvm_manager.h"
#include "ui/fonts/GeistRegular.h"
#include "features/module/module_manager.h"
#include "features/module/modules/render/esp.h"
#include "hooks/detours/native/wglSwapBuffers.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);


void gui::init(ImGuiContext *ctx, HDC device_, HWND h_window) {
    imgui_ctx = ctx;
    device = device_;

    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;

    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->AddFontFromMemoryTTF(GeistRegular, sizeof(GeistRegular), 15.f, &config);
    io.Fonts->AddFontFromMemoryTTF(GeistRegular, sizeof(GeistRegular), 17.f, &config);
    io.Fonts->AddFontFromMemoryTTF(GeistMedium, sizeof(GeistMedium), 15.f, &config);
    io.Fonts->AddFontFromMemoryTTF(GeistMedium, sizeof(GeistMedium), 17.f, &config);

    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    ImGui_ImplWin32_Init(h_window);
    ImGui_ImplOpenGL3_Init();

    ImGui::StyleColorsDark();
}


void gui::destroy() {
    if (!imgui_ctx)  return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
    imgui_ctx = nullptr;
}


void gui::render_title_bar() {
    const auto win_pos = ImGui::GetWindowPos();
    const auto win_bbox = ImRect(win_pos, win_pos + ImVec2(720.f, 840.f));
    const auto draw_list = ImGui::GetWindowDrawList();

    const auto title_bbox = ImRect(win_bbox.Min, ImVec2(win_bbox.Max.x, win_bbox.Min.y + 30.f));
    const auto version_text = " v" + std::string(bb::version);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);

    const auto title_size = ImGui::CalcTextSize("Boost Biscuit");
    const float startX = title_bbox.GetCenter().x - (title_size.x + ImGui::CalcTextSize(version_text.c_str()).x) / 2.f;
    const float centerY = title_bbox.GetCenter().y - title_size.y / 2.f;

    draw_list->AddText(ImVec2(startX, centerY), prim_text_color, "Boost Biscuit");
    draw_list->AddText(ImVec2(startX + title_size.x, centerY), sec_text_color, version_text.c_str());

    draw_list->AddLine(ImVec2(title_bbox.Min.x - 5.f, title_bbox.Max.y), ImVec2(title_bbox.Max.x + 5.f, title_bbox.Max.y), border_color, 1);

    ImGui::PopFont();
}


void gui::render_render_category() {
    constexpr float px = 10.f;

    // BEGIN ESP MODULE
    const auto esp_module = (esp *)module_manager::get().get_module("ESP");
    widgets::begin_child("ESP", 345.f, px);  // half of 720 - x padding - middle gap
    const auto *child_window = ImGui::GetCurrentWindow();

    widgets::color_edit("Box color", esp::color, child_window->Size.x - px * 2);
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(px, 10.f));

    widgets::combo("Render mode", &esp::render_mode, { "2D", "3D" });
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(px, 10.f));

    widgets::checkbox("Only render invisible", &esp::only_render_invisible, child_window->Size.x - px * 2);
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(px, 10.f));

    widgets::checkbox("Render health bar", &esp::render_health_bar, child_window->Size.x - px * 2);
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(px, 11.f));

    widgets::slider("Render distance", ImGuiDataType_U32, &esp::max_distance_value, &esp::min_distance, &esp::max_distance, "%d blocks", child_window->Size.x - px * 2);

    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(px, 15.f));
    widgets::end_child("ESP", &esp_module->keybind, &esp_module->enabled);
    // END ESP MODULE
}


void gui::render_menu(const GLint viewport[4]) {
    static float bg_alpha = 0.f;
    if (is_first_frame) {
        bg_alpha = 0;
        is_first_frame = false;
    }

    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0.F, 0.F), ImVec2{ (float)viewport[2] , (float)viewport[3] }, IM_COL32(15, 15, 15, 127 * bg_alpha));
    bg_alpha = ImLerp(bg_alpha, 1.f, ImGui::GetIO().DeltaTime * 20.f);

    ImGui::SetNextWindowBgAlpha(1.f);
    ImGui::SetNextWindowSize(ImVec2(720.f, 840.f));
    ImGui::SetNextWindowPos(ImVec2(viewport[2] / 2 - 360.f, viewport[3] / 2 - 420.f), ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(9.f/255.f, 9.f/255.f, 9.f/255.f, 1.f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.F, 0.F));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.F, 1.F));

    ImGui::Begin("main", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

    render_title_bar();

    // we have to do spacing here because navbar does not call AddItem() like a responsible widget :(
    ImGui::Spacing();
    ImGui::Dummy(ImVec2(0, 30));

    static int active_button_index = 0;
    constexpr ImVec2 navbar_size{700.f, 40.f};
    widgets::navbar({ "Render", "Player", "Combat", "Client", "Legit" }, navbar_size, &active_button_index);

    const auto line_start = ImVec2(0, ImGui::GetCursorScreenPos().y);
    const auto line_end = ImVec2(3000, ImGui::GetCursorScreenPos().y);
    ImGui::GetWindowDrawList()->AddLine(line_start, line_end, border_color, 1.0f);

    // padding before modules
    ImGui::Spacing();
    ImGui::Dummy(ImVec2(0, 10));

    switch (active_button_index) {
        case 0: render_render_category(); break;
        case 1: /* render_player_category(); */ break;
        case 2: /* render_combat_category(); */ break;
        case 3: /* render_client_category(); */ break;
        case 4: /* render_legit_category(); */ break;
    }

    ImGui::End();

    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(4);
}


// make sure movement keys are released when menu is opened
void release_key(const int vk) {
    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}


void on_menu_open() {
    release_key('W');
    release_key('A');
    release_key('S');
    release_key('D');
    release_key('V');  // this one is for me :)
    release_key(VK_SPACE);
    release_key(VK_SHIFT);
    release_key(VK_CONTROL);
}


void gui::render(const GLint viewport[4]) {
    const auto gl_ctx = wglGetCurrentContext();
    wglMakeCurrent(device, gl_ctx);

    ImGui::GetIO().DisplaySize = ImVec2{ static_cast<float>(viewport[2]) , static_cast<float>(viewport[3]) };

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    {
        JNIEnv *env;
        if (jvm_manager::get().get_jvm()->AttachCurrentThread((void **)&env, nullptr) != JNI_OK) {
            return logger::get().error("Failed to attach current thread to JVM in wglSwapBuffers_detour!");
        }

        for (const auto& module : module_manager::get().get_modules()) {
            if (module->is_enabled()) {
                module->on_swap_buffers(env);  // it just works here
                module->on_imgui_render(viewport);
            }
        }
    }

    // render gui last (on top)
    if (is_being_drawn) {
        if (is_first_frame)
            on_menu_open();

        render_menu(viewport);
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    wglMakeCurrent(device, gl_ctx);
}


// return true to block input to the game
bool gui::WndProc_handler(HWND h_window, const UINT msg, const WPARAM w_param, const LPARAM l_param) {
    // for setting keybinds
    if (WndProc_callback != nullptr && (msg == WM_KEYDOWN || msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_XBUTTONDOWN)) {
        WndProc_callback(h_window, msg, w_param, l_param, keybind_ptr);
        WndProc_callback = nullptr;
        keybind_ptr = nullptr;
        return true;
    }

    bool is_esc_pressed = false;
    if (msg == WM_KEYDOWN && w_param == gui_bind)
        is_being_drawn = !is_being_drawn;

    if (msg == WM_KEYDOWN && w_param == VK_ESCAPE && is_being_drawn) {
        is_being_drawn = false;
        is_esc_pressed = true;
    }

    if (is_being_drawn) {
        ImGui_ImplWin32_WndProcHandler(h_window, msg, w_param, l_param);

        return msg != WM_KEYUP;  // let key release events through so on_menu_open works
    }

    is_first_frame = true;

    return is_esc_pressed;  // no open ingame menu when esc pressed to quit imgui menu!
}
