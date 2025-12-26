
#include "nametags.h"

#include <cfloat>

#include "imgui.h"
#include "features/module/common_data.h"
#include "utils/render.h"
#include "wrapper/minecraft/entity/EntityLivingBase.h"


void nametags::on_render_name(JNIEnv *env, jobject obj, bool *cancel) {
    const auto entity = wrapper::EntityLivingBase(env, obj);
    if (entity.is_player())
        *cancel = true;
}


void nametags::on_imgui_render(const GLint viewport[4]) {
    std::lock_guard lock(common_data::esp_render_data_mutex);
    if (common_data::esp_render_data.projection.empty() || common_data::esp_render_data.model_view.empty())
        return logger::get().warn("proj or model_view matrix not set, skipping esp render!!");

    for (auto& entity_ss : common_data::esp_render_data.entities) {
        float box_left = FLT_MAX;
        float box_top = FLT_MAX;
        float box_right = FLT_MIN;
        float box_bottom = FLT_MIN;
        bool skip = false;

        for (const auto &bbox_corner : entity_ss.bbox_corners) {  // skip first two points
            vec2 screen_pos{};
            if (render_utils::world2screen(bbox_corner, common_data::esp_render_data.model_view, common_data::esp_render_data.projection, viewport, screen_pos)) {
                if (screen_pos.x < box_left)      box_left = (float)screen_pos.x;
                if (screen_pos.x > box_right)     box_right = (float)screen_pos.x;
                if (screen_pos.y < box_top)       box_top = (float)screen_pos.y;
                if (screen_pos.y > box_bottom)    box_bottom = (float)screen_pos.y;
            } else {
                skip = true;
                break;
            }
        }

        if (skip)  continue;

        ImGui::GetBackgroundDrawList()->AddText(ImVec2(box_left - 100.f, box_top - 50.f), IM_COL32(255, 255, 255, 255), "player");
    }
}
