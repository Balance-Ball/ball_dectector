#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <opencv2/core/types.hpp>
#include <utility>
#include <opencv2/opencv.hpp>

namespace ball_tracker {
    using BinaryRange = std::pair<cv::Scalar, cv::Scalar>;

    struct Config {
        [[nodiscard]] static auto load_config(std::filesystem::path path) -> Config;
    private:
        std::array<uint16_t, 3> ball_hsv_begin = {};
        std::array<uint16_t, 3> ball_hsv_end   = {};
        std::array<uint16_t, 3> bg_hsv_begin = {};
        std::array<uint16_t, 3> bg_hsv_end   = {};

    public:
        // Optional options
        std::array<uint16_t, 2> resolution = {};
        uint8_t video_dev_index = 0;

        uint8_t open_kernal_size  = 0u;
        uint8_t close_kernal_size = 0u;
        uint16_t circle_min_area  = 0u;
        uint16_t circle_max_area  = 0u;
        float circularity = 0.f;

        [[nodiscard]] inline auto get_ball_range() const -> BinaryRange {
            auto begin = cv::Scalar(
                this->ball_hsv_begin[0],
                this->ball_hsv_begin[1],
                this->ball_hsv_begin[2]
            );

            auto end = cv::Scalar(
                this->ball_hsv_end[0],
                this->ball_hsv_end[1],
                this->ball_hsv_end[2]
            );
            return std::make_pair(std::move(begin), std::move(end));
        }

        [[nodiscard]] inline auto get_bg_range() const -> BinaryRange {
            auto begin = cv::Scalar(
                this->bg_hsv_begin[0],
                this->bg_hsv_begin[1],
                this->bg_hsv_begin[2]
            );

            auto end = cv::Scalar(
                this->bg_hsv_end[0],
                this->bg_hsv_end[1],
                this->bg_hsv_end[2]
            );

            return std::make_pair(begin, end);
        }

    };

    
}
