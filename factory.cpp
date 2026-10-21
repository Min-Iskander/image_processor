//ok
#include "factory.h"

#include <cmath>
#include <iomanip>
#include <map>
#include <string>
#include <limits>

#include "errors.h"
#include "filters/crop.h"
#include "filters/edge_detection.h"
#include "filters/gaussian_blur.h"
#include "filters/grayscale.h"
#include "filters/negative.h"
#include "filters/pinch.h"
#include "filters/sharpening.h"

namespace {
    void ExpectParams(const Filterinfo& spec, size_t count) {
        if (spec.params.size() != count) {
            throw FilterParameterError("У -" + spec.name + " ожидается " + std::to_string(count) +
                                     " параметров, получено " + std::to_string(spec.params.size()));
        }
    }

    size_t ParseSize(const std::string& value) {
        if (value.empty()) {
            throw FilterParameterError("Ожидалось целое число, получена пустая строка");
        }
        const size_t max = std::numeric_limits<size_t>::max();
        size_t result = 0;
        for (char c : value) {
            if (c < '0' || c > '9') {
                throw FilterParameterError("Ожидалось целое неотрицательное число, получено: " + value);
            }
            const size_t digit = c - '0';
            if (result > (max - digit) / 10) {
                throw FilterParameterError("Получено слишком большое число " + value);
            }
            result = result * 10 + digit;
        }
        return result;
    }

    float ParseFloat(const std::string& value) {
        size_t parsed = 0;
        float result = 0.0f;
        try {
            result = std::stof(value, &parsed);
        } catch (const std::exception&) {
            throw FilterParameterError("Ожидалось число, получено " + value);
        }
        if (parsed != value.size() || !std::isfinite(result)) {
            throw FilterParameterError("Ожидалось число, получено " + value);
        }
        return result;
    }

    std::unique_ptr<Filter> CreateCrop(const Filterinfo& spec) {
        ExpectParams(spec, 2);
        return std::make_unique<Crop>(ParseSize(spec.params[0]), ParseSize(spec.params[1]));
    }

    std::unique_ptr<Filter> CreateGrayscale(const Filterinfo& spec) {
        ExpectParams(spec, 0);
        return std::make_unique<Grayscale>();
    }

    std::unique_ptr<Filter> CreateNegative(const Filterinfo& spec) {
        ExpectParams(spec, 0);
        return std::make_unique<Negative>();
    }

    std::unique_ptr<Filter> CreateSharpening(const Filterinfo& spec) {
        ExpectParams(spec, 0);
        return std::make_unique<Sharpening>();
    }

    std::unique_ptr<Filter> CreateEdgeDetection(const Filterinfo& spec) {
        ExpectParams(spec, 1);
        return std::make_unique<EdgeDetection>(ParseFloat(spec.params[0]));
    }

    std::unique_ptr<Filter> CreateGaussianBlur(const Filterinfo& spec) {
        ExpectParams(spec, 1);
        return std::make_unique<GaussianBlur>(ParseFloat(spec.params[0]));
    }

    std::unique_ptr<Filter> CreatePinch(const Filterinfo& spec) {
        ExpectParams(spec, 1);
        return std::make_unique<Pinch>(ParseFloat(spec.params[0]));
    }

    using Creator = std::unique_ptr<Filter> (*)(const Filterinfo& spec);

    struct FilterInfo {
        std::string usage;
        std::string desc;
        Creator create;
    };

    const std::map<std::string, FilterInfo> FILTERS = {
            {"blur", {"-blur sigma", "гауссово размытие", CreateGaussianBlur}},
            {"crop", {"-crop width height", "обрезать до верхнего левого угла width x height", CreateCrop}},
            {"edge", {"-edge threshold", "выделение границ", CreateEdgeDetection}},
            {"gs", {"-gs", "оттенки серого", CreateGrayscale}},
            {"neg", {"-neg", "негатив", CreateNegative}},
            {"pinch", {"-pinch scale", "щипок, scale", CreatePinch}},
            {"sharp", {"-sharp", "повышение резкости", CreateSharpening}},
    };

}  // namespace

std::unique_ptr<Filter> CreateFilter(const Filterinfo& spec) {
    const auto it = FILTERS.find(spec.name);
    if (it == FILTERS.end()) {
        throw UnknownFilterError("Неизвестный фильтр: -" + spec.name);
    }
    return it->second.create(spec);
}

void PrintFiltersHelp(std::ostream& out) {
    for (const auto& [name, info] : FILTERS) {
        out << "  " << info.usage << " — " << info.desc << '\n';
    }
}