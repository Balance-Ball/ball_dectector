#include "config.hpp"
#include "nlohmann_json.hpp"
#include <array>
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace {
    constexpr std::array<uint16_t, 2> DEFAULT_FRAME_PROC_RESOLUTION = {640, 480};
    constexpr uint8_t DEFAULT_VIDEO_DEVICE_INDEX = 0;

    constexpr uint8_t DEFAULT_OPEN_KERNAL_SIZE  = 3;
    constexpr uint8_t DEFAULT_CLOSE_KERNAL_SIZE = 9;
    constexpr uint16_t DEFAULT_CIRCLE_MIN_AREA  = 200;
    constexpr uint16_t DEFAULT_CIRCLE_MAX_AREA  = 2000;
    constexpr float DEFAULT_MIN_CIRCULARITY = 0.8f;
}

namespace ball_tracker {
    auto load_config(std::filesystem::path path) -> Config {
        auto file_reader = std::ifstream(path);
        if (!file_reader.is_open()) {
            throw std::logic_error("Failed to open config file");
        }

        auto json = nlohmann::json::parse(file_reader);
        auto result = Config();

        // Required options
        json.at("ball_hsv_begin").get_to(result.ball_hsv_begin);
        json.at("ball_hsv_end")  .get_to(result.ball_hsv_end);
        json.at("bg_hsv_begin").get_to(result.bg_hsv_begin);
        json.at("bg_hsv_end")  .get_to(result.bg_hsv_end);

        // Optional options
        result.resolution = json.value<std::array<uint16_t, 2>>("resolution", DEFAULT_FRAME_PROC_RESOLUTION);
        result.video_dev_index = json.value<uint8_t>("video_dev_index", DEFAULT_VIDEO_DEVICE_INDEX);

        result.open_kernal_size  = json.value<uint8_t>("open_kernal_size", DEFAULT_OPEN_KERNAL_SIZE);
        result.close_kernal_size = json.value<uint8_t>("close_kernal_size", DEFAULT_CLOSE_KERNAL_SIZE);
        result.circle_min_area   = json.value<uint16_t>("circle_min_area", DEFAULT_CIRCLE_MIN_AREA);
        result.circle_max_area   = json.value<uint16_t>("circle_max_area", DEFAULT_CIRCLE_MAX_AREA);
        result.circularity = json.value<float>("min_circularity", DEFAULT_MIN_CIRCULARITY);

        return result;
    }
}
