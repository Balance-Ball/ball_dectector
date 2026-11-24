#pragma once

#include <array>
#include <cstdint>
#include <filesystem>

namespace ball_tracker {
    struct Config {
        std::array<uint16_t, 3> filter_begin = {};
        std::array<uint16_t, 3> filter_end   = {};

        // Optional options
        std::array<uint16_t, 2> resolution = {};
        uint8_t video_dev_index = 0;

        uint8_t open_kernal_size  = 0u;
        uint8_t close_kernal_size = 0u;
        uint16_t circle_min_area  = 0u;
        uint16_t circle_max_area  = 0u;
        float circularity = 0.f;
    };

    auto load_config(std::filesystem::path path) -> Config;
}
