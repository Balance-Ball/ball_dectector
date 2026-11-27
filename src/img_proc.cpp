#include "img_proc.hpp"
#include "config.hpp"
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/geometry.hpp>
#include <opencv2/geometry/2d.hpp>
#include <opencv2/opencv.hpp>
#include <utility>
#include <vector>
#include <cmath>
#include <optional>

namespace {
    struct DetectedBlob {
        double area = 0.f;
        double perimeter  = 0.f;
        float circularity = 0.f;

        std::vector<cv::Point> points;
    };

    static auto preproc(const cv::Mat input, cv::Mat& output, const ball_tracker::BinaryRange bin_range, ball_tracker::Config cfg) -> void {
        auto binary = cv::Mat();
        auto cleaned = cv::Mat();
        
        ball_tracker::binary(input, binary, bin_range);
        ball_tracker::clean(binary, cleaned, cfg);

        output = cleaned;
    }
}

namespace ball_tracker {
    auto binary(const cv::Mat input, cv::Mat& output, BinaryRange range) -> void {
        auto gaus = cv::Mat();
        cv::GaussianBlur(input, gaus, cv::Size(3, 3), 5);
        cv::inRange(gaus, range.first, range.second, output);
    }

    auto clean(const cv::Mat input, cv::Mat& output, const Config& cfg) -> void {
        auto opened = cv::Mat();
        auto closed = cv::Mat();
        
        auto opened_kernal = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(cfg.open_kernal_size, cfg.open_kernal_size));
        auto closed_kernal = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(cfg.close_kernal_size, cfg.close_kernal_size));
        cv::morphologyEx(input, opened, cv::MORPH_OPEN, opened_kernal);
        cv::morphologyEx(opened, output, cv::MORPH_CLOSE, closed_kernal);
    }

    auto detect_balls(const cv::Mat input, const Config& cfg) -> std::optional<std::vector<BallDetectResult>> {
        auto proc_frame = cv::Mat();
        preproc(input, proc_frame, cfg.get_ball_range(), cfg);

        std::vector<std::vector<cv::Point>> contour_points = {};
        cv::findContours(proc_frame, contour_points,
            cv::RETR_EXTERNAL,
            cv::CHAIN_APPROX_NONE
        );

        // find circle blobs
        auto detected_blobs = std::vector<DetectedBlob>();
        for (const auto& contour : contour_points) {
            const double area = cv::contourArea(contour);
            if (area < cfg.circle_min_area || area > cfg.circle_max_area) continue;

            const double perimeter  = cv::arcLength(contour, true);
            // circularity = 4 * PI * Area / perimeter ^ 2
            const float circularity = 4.f * CV_PI * area / std::pow(perimeter, 2);
            if (circularity < cfg.circularity) continue;

            detected_blobs.push_back(DetectedBlob {
                .area = area,
                .perimeter = perimeter,
                .circularity = circularity,
                .points = std::move(contour)
            });
        }

        if (detected_blobs.empty()) return std::nullopt;

        auto result = std::vector<BallDetectResult>(detected_blobs.size());
        for (std::size_t i = 0u; i < detected_blobs.size(); ++i) {
            const auto& blob = detected_blobs[i];
            auto center_pos = cv::Point2f();
            auto radius = 0.f;
            
            cv::minEnclosingCircle(blob.points, center_pos, radius);

            result[i].x = center_pos.x;
            result[i].y = center_pos.y;
            result[i].r = radius;
        }
        


        return result;
    }

    auto detect_background(const cv::Mat input, const Config& cfg) -> std::optional<BgDetectResult> {
        auto proc_frame = cv::Mat();
        preproc(input, proc_frame, cfg.get_bg_range(), cfg);

        std::vector<std::vector<cv::Point>> contour_points = {};
        cv::findContours(proc_frame, contour_points,
            cv::RETR_EXTERNAL,
            cv::CHAIN_APPROX_NONE
        );

        if (contour_points.empty()) return std::nullopt;

        auto center_pos = cv::Point2f();
        auto radius = 0.f;
        
        auto lagrest = std::max_element(contour_points.begin(), contour_points.end(),
            [](const auto& a, const auto& b) {
                return cv::contourArea(a) < cv::contourArea(b);
            }
        );

        cv::minEnclosingCircle(*lagrest, center_pos, radius);
        
        auto result = BgDetectResult {
            .x = center_pos.x,
            .y = center_pos.y,
            .r = radius
        };

        return result;
    }

    auto proc_frame(const cv::Mat raw_frame, const Config& proc_cfg) -> std::optional<std::vector<BallDetectResult>> {
        auto proc_frame = cv::Mat();
        cv::cvtColor(raw_frame, proc_frame, cv::COLOR_BGR2HSV);

        return ball_tracker::detect_balls(proc_frame, proc_cfg);
    }
}
