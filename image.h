//ok
#pragma once

#include <cstddef>
#include <vector>

#include "color.h"

class Image {
private:
    size_t width_ = 0;
    size_t height_ = 0;
    std::vector<Color> pixels_;
public:
    Image(size_t width, size_t height);

    size_t Width() const;

    size_t Height() const;

    Color &At(size_t x, size_t y);

    const Color &At(size_t x, size_t y) const;
};