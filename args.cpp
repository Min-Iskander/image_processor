//ok
#include "args.h"
#include "errors.h"
#include <cctype>

namespace {
    bool Isfiltername(const std::string &arg) {
        return arg.size() >= 2 && arg[0] == '-' && std::isalpha(static_cast<unsigned char>(arg[1]));
    }
}

Args ParseArgs(int argc, char **argv) {
    if (argc < 3) {
        throw UsageError("Нужно указать входной и выходной файл");
    }
    Args args;
    args.input_path = argv[1];
    args.output_path = argv[2];
    for (int i = 3; i < argc; ++i) {
        const std::string arg = argv[i];
        if (Isfiltername(arg)) {
            args.filters.push_back({arg.substr(1, arg.size() - 1), {}});
        } else if (args.filters.empty()) {
            throw UsageError("Получен параметр без фильтра: " + arg);
        } else {
            args.filters.back().params.push_back(arg);
        }
    }
    return args;
}