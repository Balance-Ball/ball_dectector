#pragma once

#include <string>

namespace ball_tracker {
    struct CmdOptions {
        bool help = false;
        std::string source = "";
    };

    auto parse_args(int argc, char* argv[]) -> CmdOptions;
}
