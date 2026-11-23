#pragma once

#include <string>

namespace ball_tracker {
    struct CmdOptions {
        std::string source = "";
    };

    auto parse_args(int argc, char* argv[]) -> CmdOptions;
}
