//ok
#pragma once

#include <string>
#include <vector>

struct Filterinfo {
    std::string name;
    std::vector<std::string> params;
};

struct Args {
    std::string input_path;
    std::string output_path;
    std::vector<Filterinfo> filters;
};

Args ParseArgs(int argc, char **argv);