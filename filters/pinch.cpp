#include "pinch.h"
#include "../errors.h"

#include <algorithm>
#include <cmath>

namespace {
    Color Mix(const Color &from, const Color &to, float t) {
        Color result;
        result.r = from.r + (to.r - from.r) * t;
        result.g = from.g + (to.g - from.g) * t;
        result.b = from.b + (to.b - from.b) * t;
        return result;
    }

    Color Sample(const Image &image, float x, float y) {
        x = std::min(std::max(x, 0.0f), static_cast<float>(image.Width() - 1));
        y = std::min(std::max(y, 0.0f), static_cast<float>(image.Height() - 1));
        const auto x0 = static_cast<size_t>(x);
        const auto y0 = static_cast<size_t>(y);
        const size_t x1 = std::min(x0 + 1, image.Width() - 1);
        const size_t y1 = std::min(y0 + 1, image.Height() - 1);
        const float tx = x - static_cast<float>(x0);
        const float ty = y - static_cast<float>(y0);
        const Color top = Mix(image.At(x0, y0), image.At(x1, y0), tx);
        const Color bot = Mix(image.At(x0, y1), image.At(x1, y1), tx);
        return Mix(top, bot, ty);
    }

}  // namespace

Pinch::Pinch(float scale) : scale_(scale) {
    if (scale_ < -1.0f || scale_ > 1.0f) {
        throw FilterParameterError("-pinch: scale должен быть от -1 до 1");
    }
}

Image Pinch::Apply(Image &&image) const {
    const auto width = static_cast<float>(image.Width());
    const auto height = static_cast<float>(image.Height());
    const float center_x = (width - 1) / 2;
    const float center_y = (height - 1) / 2;
    const float radius = std::min(width, height) / 2;
    Image result(image.Width(), image.Height());
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            const float dx = static_cast<float>(x) - center_x;
            const float dy = static_cast<float>(y) - center_y;
            const float distance = std::sqrt(dx * dx + dy * dy);
            if (distance == 0.0f || distance >= radius) {
                result.At(x, y) = image.At(x, y);
                continue;
            }
            const float stretch = std::pow(std::sin(acosf(-1.0f) / 2 * distance / radius), -scale_);
            const float from_distance = std::min(distance * stretch, radius);
            const float from_x = center_x + dx / distance * from_distance;
            const float from_y = center_y + dy / distance * from_distance;
            result.At(x, y) = Sample(image, from_x, from_y);
        }
    }
    return result;
}