
#include "esp.h"

#include <cfloat>
#include <bits/stl_algo.h>

#include "utils/render.h"
#include "utils/logs/logger.h"
#include "features/module/common_data.h"


void draw_health_bar(const float distance, const float box_left, const float box_top, const float box_bottom, const float hp) {
	const float dist = std::clamp(distance, 1.5f, 80.0f);
	const float scale = 1.0f / std::sqrt(dist * 0.35f);

	const float health_bar_width = std::clamp(6.f * scale, 4.f, 10.f);  // gpt wanted 2.f and 8.f
	const float health_bar_offset = std::clamp(8.f + health_bar_width + 4.f, 6.f, 8.f);
	constexpr float border_width = 1.f;

	const ImVec2 min(box_left - health_bar_offset, box_top);
	const ImVec2 max(box_left - health_bar_offset + health_bar_width, box_bottom);

	auto* draw = ImGui::GetBackgroundDrawList();

	draw->AddRectFilled(min, max, IM_COL32(0, 0, 0, 255), 0.f, 0);

	const float height = max.y - min.y - border_width * 2;
	const float filled = height * hp;
	draw->AddRectFilled(
		ImVec2(min.x + border_width, max.y - border_width - filled),
		ImVec2(max.x - border_width, max.y - border_width),
		ImColor::HSV(hp * 0.33f, 1.0f, 1.0f)
	);
}


bool filter_player(const entity_snapshot &ss) {
	return (esp::only_render_invisible && !ss.is_invisible) || (ss.distance > (float)esp::max_distance_value);
}


void esp::on_imgui_render(const GLint viewport[4]) {
	if (render_mode != RECTANGLE && !render_health_bar)
		return;

    std::lock_guard lock(common_data::esp_render_data_mutex);
    if (common_data::esp_render_data.projection.empty() || common_data::esp_render_data.model_view.empty())
        return logger::get().warn("proj or model_view matrix not set, skipping esp render!!");

    for (auto& entity_ss : common_data::esp_render_data.entities) {
    	if (filter_player(entity_ss))
    		continue;

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

    	if (render_mode == RECTANGLE)
			ImGui::GetBackgroundDrawList()->AddRect(ImVec2(box_left, box_top), ImVec2(box_right, box_bottom),
        		IM_COL32(color[0] * 255.f, color[1] * 255.f, color[2] * 255.f, color[3] * 255.f), 0, 0, 1.f);

    	if (render_health_bar)
    		draw_health_bar(entity_ss.distance, box_left, box_top, box_bottom, std::clamp(entity_ss.health / entity_ss.max_health, 0.0f, 1.0f));
    }
}


void draw_outlined_box(const wrapper::bbox_coords &bbox) {
	glBegin(3);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);  // NOLINT
	glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);  // NOLINT
	glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);  // NOLINT
	glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);  // NOLINT
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);  // NOLINT
	glEnd();

	glBegin(3);
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);  // NOLINT
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);  // NOLINT
	glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);  // NOLINT
	glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);  // NOLINT
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);  // NOLINT
	glEnd();

	glBegin(1);
	glVertex3f(bbox.minX, bbox.minY, bbox.minZ);  // NOLINT
	glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);  // NOLINT
	glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);  // NOLINT
	glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);  // NOLINT
	glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);  // NOLINT
	glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);  // NOLINT
	glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);  // NOLINT
	glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);  // NOLINT
	glEnd();
}


void esp::on_swap_buffers(JNIEnv *env) {
	if (render_mode != HITBOX)
		return;

	std::lock_guard lock(common_data::esp_render_data_mutex);

	if (common_data::esp_render_data.projection.empty() || common_data::esp_render_data.model_view.empty())
		return logger::get().warn("proj or model_view matrix not set, skipping 3d esp render!!");

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(common_data::esp_render_data.projection.data());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(common_data::esp_render_data.model_view.data());

	glPushMatrix();
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glLineWidth(1.f);

	glColor4f(color[0], color[1], color[2], color[3]);
	for (const auto &ss : common_data::esp_render_data.entities) {
		if (filter_player(ss))
			continue;

		draw_outlined_box(ss.bbox);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);
	glPopMatrix();

	glPopMatrix();
}
