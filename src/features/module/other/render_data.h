
#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include <vector>

#include "utils/structs/vec3.h"
#include "wrapper/minecraft/util/AxisAlignedBB.h"


struct entity_snapshot {
    wrapper::bbox_coords bbox{};
    vec3 bbox_corners[8];
    float distance = 0.f;
    float health = 0.f;
    float max_health = 1.f;  // prevent division by zero
    bool is_invisible = false;
};


struct esp_snapshot {
    std::vector<float> projection;
    std::vector<float> model_view;
    std::vector<entity_snapshot> entities;
};


void update_render_data();


#endif //RENDER_DATA_H
