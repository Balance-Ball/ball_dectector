#pragma once

#include "config.hpp"
#include <opencv2/opencv.hpp>
#include <optional>

namespace ball_tracker {
    struct DetectResult {
        float x = 0u, y = 0u;
        float r = 0.f;
    };

    using BallDetectResult = DetectResult;
    using BgDetectResult   = DetectResult; 

    auto binary(cv::Mat input, cv::Mat& output, BinaryRange range) -> void;
    auto clean(const cv::Mat input, cv::Mat& output, const Config& cfg) -> void;
    auto detect_balls(const cv::Mat input, const Config& cfg) -> std::optional<std::vector<BallDetectResult>>;
    auto detect_background(const cv::Mat input, const Config& cfg) -> std::optional<BgDetectResult>;
    auto proc_frame(const cv::Mat raw_frame, const Config& cfg) -> std::optional<std::vector<BallDetectResult>>;
}
