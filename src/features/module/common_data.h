
#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <mutex>

#include "other/render_data.h"


namespace common_data {
    inline esp_snapshot esp_render_data{};
    inline auto esp_render_data_mutex = std::mutex{};
}


#endif //COMMON_DATA_H
