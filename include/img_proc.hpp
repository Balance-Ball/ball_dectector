#pragma once

#include "config.hpp"
#include <opencv2/opencv.hpp>
#include <cstdint>
#include <optional>

namespace ball_tracker {
    struct DetectResult {
        uint32_t x = 0u, y = 0u;
        float r = 0.f;
    };

    auto binary(cv::Mat input, cv::Mat& output, const Config& cfg) -> void;
    auto clean(const cv::Mat input, cv::Mat& output, const Config& cfg) -> void;
    auto detect_balls(const cv::Mat input, const Config& cfg) -> std::optional<std::vector<DetectResult>>;
    auto proc_frame(const cv::Mat raw_frame, const Config& cfg) -> std::optional<std::vector<DetectResult>>;
}
