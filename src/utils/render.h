
#ifndef RENDER_H
#define RENDER_H

#include "GL/gl.h"
#include <vector>

#include "utils/structs/vec2.h"
#include "utils/structs/vec3.h"
#include "utils/structs/vec4.h"


class render_utils {
public:
    static vec4 multiply(const vec4 &vec, const std::vector<float> &matrix) {
        return {
            vec.x * matrix[0] + vec.y * matrix[4] + vec.z * matrix[8] + vec.w * matrix[12],
            vec.x * matrix[1] + vec.y * matrix[5] + vec.z * matrix[9] + vec.w * matrix[13],
            vec.x * matrix[2] + vec.y * matrix[6] + vec.z * matrix[10] + vec.w * matrix[14],
            vec.x * matrix[3] + vec.y * matrix[7] + vec.z * matrix[11] + vec.w * matrix[15]
        };
    }

    static bool world2screen(const vec3 &world_pos, const std::vector<float> &model_view, const std::vector<float> &projection, const GLint viewport[4], vec2 &screen_pos) {
        vec4 clip_space_pos = multiply(vec4(world_pos.x, world_pos.y, world_pos.z, 1.0f), model_view);
        clip_space_pos = multiply(clip_space_pos, projection);

        if (clip_space_pos.w < 0.1f)
            return false;

        const vec3 ndc = {
            clip_space_pos.x / clip_space_pos.w,
            clip_space_pos.y / clip_space_pos.w,
            clip_space_pos.z / clip_space_pos.w
        };

        screen_pos.x = ((ndc.x + 1.0f) / 2.0f) * viewport[2] + viewport[0];
        screen_pos.y = ((1.0f - ndc.y) / 2.0f) * viewport[3] + viewport[1];

        return true;
    }
};


#endif //RENDER_H
