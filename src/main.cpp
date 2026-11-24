#include "img_proc.hpp"
#include "cmdline.hpp"
#include "config.hpp"
#include <cstdio>
#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

constexpr const char* USAGE = 
    "ball_detector --help\n"
"   --source <image or video device>\n"
"   --display <true/false>\n";

auto main(int argc, char* argv[]) -> int {
    const ball_tracker::CmdOptions opts = ball_tracker::parse_args(argc, argv);
    
    if (opts.help) {
        std::printf("%s", USAGE);
        return 0;
    }

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
        cv::resize(frame, frame, cv::Size(proc_cfg.resolution[0], proc_cfg.resolution[1]));
        auto display = frame.clone();

        auto balls = ball_tracker::proc_frame(frame, proc_cfg);
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
