#include "img_proc.hpp"
#include "cmdline.hpp"
#include "config.hpp"
#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

auto main(int argc, char* argv[]) -> int {
    const ball_tracker::CmdOptions opts = ball_tracker::parse_args(argc, argv);
    const ball_tracker::Config proc_cfg = ball_tracker::load_config("config/config.json");

    cv::namedWindow("find_balls", cv::WINDOW_NORMAL);

    auto cam = cv::VideoCapture(proc_cfg.video_dev_index);
    if (!cam.isOpened()) {
        std::cerr << "Failed to open video device\n";
        return -1;
    }

    while(cv::waitKey(1) != 'q') {
        auto frame = cv::Mat();
        cam.read(frame);
        cv::resize(frame, frame, cv::Size(frame.cols / 4, frame.rows / 4));
        auto display = frame.clone();
        auto proc_frame = cv::Mat();
        cv::cvtColor(frame, proc_frame, cv::COLOR_BGR2HSV);

        auto binary = cv::Mat();
        ball_tracker::binary(proc_frame, binary, proc_cfg);
        auto cleaned = cv::Mat();
        ball_tracker::clean(binary, cleaned, proc_cfg);

        auto balls = ball_tracker::detect_balls(cleaned, proc_cfg);
        if (balls.has_value()) {
            for (const auto& ball : balls.value()) {
                auto detect_box = cv::Rect(ball.x - ball.r, ball.y - ball.r, 2 * ball.r, 2 * ball.r);
                cv::rectangle(display, detect_box, cv::Scalar(0, 255, 0), 1);
            }
        }

        cv::imshow("find_balls", display);
    }

    return 0;
}
