
#include "wglSwapBuffers.h"

#include "ui/gui.h"
#include "boostbiscuit.h"
#include "utils/logs/logger.h"
#include "utils/render.h"
#include "wrapper/minecraft/client/Minecraft.h"
#include "features/module/module_manager.h"


BOOL wglSwapBuffers::wglSwapBuffers_detour(HDC hdc) {
    static bool imgui_init = false;
    static HWND h_window = bb::h_window;

    if (GetObjectType(hdc) == 0)
        return wglSwapBuffers_original(hdc);

    gui &gui_instance = gui::get();

    if (imgui_init && h_window != WindowFromDC(hdc) || !bb::running.load()) {
        gui_instance.destroy();
        imgui_init = false;

        SetWindowLongPtr(h_window, GWLP_WNDPROC, WndProc_original);

        return wglSwapBuffers_original(hdc);
    }

    if (!imgui_init) {
        h_window = WindowFromDC(hdc);

        gui_instance.init(ImGui::CreateContext(), hdc, h_window);
        imgui_init = true;

        WndProc_original = SetWindowLongPtr(h_window, GWLP_WNDPROC, (LONG_PTR)WndProc_detour);
    }

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    gui_instance.render(viewport);

    return wglSwapBuffers_original(hdc);
}


LRESULT CALLBACK wglSwapBuffers::WndProc_detour(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param) {
    if (gui::get().WndProc_handler(h_window, msg, w_param, l_param))  // gui returns true to block input to game
		return true;

	module_manager::get().WndProc_handler(h_window, msg, w_param, l_param);

    return CallWindowProcW((WNDPROC)WndProc_original, h_window, msg, w_param, l_param);
}


/*
static void drawFilledBox(const wrapper::bbox_coords &bbox) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(255, 255, 255, 0.15f);
	glBegin(7);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
	glEnd();
	glBegin(7);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
	glEnd();
	glBegin(7);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
	glEnd();
	glBegin(7);
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
	glEnd();
	glBegin(7);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
	glEnd();
	glBegin(7);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
	glEnd();
	glBegin(7);
	glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
	glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
	glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
	glEnd();
	glDisable(GL_BLEND);//turn the blending off 
	glColor4f(255, 255, 255, 1);
}
*/