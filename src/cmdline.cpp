#include "cmdline.hpp"
#include <opencv2/opencv.hpp>
#include <string>

namespace ball_tracker {
    static constexpr const char* opt_keys = 
        "{ help    |             | 使用帮助   }"
        "{ source  | /dev/video0 | 图像处理源 }"
        "{ display |  false | 开启图像预览 }";

    auto parse_args(int argc, char* argv[]) -> CmdOptions {
        CmdOptions opts = {};
        auto parser = cv::CommandLineParser(argc, argv, opt_keys);
        opts.help = parser.has("help");
        opts.source = parser.get<std::string>("source");
        return opts;
    }
}
