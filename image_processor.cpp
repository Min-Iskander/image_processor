//ok
#include <exception>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "args.h"
#include "bmp.h"
#include "filters/filter.h"
#include "factory.h"
#include "errors.h"

namespace {
    void PrintHelp() {
        std::cout << "Использование: image_processor <вход.bmp> <выход.bmp> [-фильтр [параметры]] ...\n"
                  << "Фильтры:\n";
        PrintFiltersHelp(std::cout);
        std::cout << "\nПример:\n"
                  << "  image_processor input.bmp output.bmp -crop 800 600 -gs -blur 0.5\n";
    }
}  // namespace

int main(int argc, char **argv) {
    if (argc == 1) {
        PrintHelp();
        return 0;
    }
    try {
        const Args args = ParseArgs(argc, argv);
        std::vector<std::unique_ptr<Filter>> filters;
        for (const Filterinfo &spec: args.filters) {
            filters.push_back(CreateFilter(spec));
        }
        Image image = ReadBmp(args.input_path);
        for (const auto &filter: filters) {
            image = filter->Apply(std::move(image));
        }
        WriteBmp(image, args.output_path);
    } catch (const ArgumentError &error) {
        std::cerr << "Ошибка в аргументах: " << error.what() << '\n';
        return 1;
    } catch (const BmpError &error) {
        std::cerr << "Ошибка в файле: " << error.what() << '\n';
        return 2;
    } catch (...) {
        std::cerr << "Неизвестная ошибка\n";
        return 3;
    }
    return 0;
}