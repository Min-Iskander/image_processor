//ok
#include "crop.h"
#include "../errors.h"

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
    if (width_ == 0 || height_ == 0) {
        throw FilterParameterError("В -crop ширина и высота должны быть положительными");
    }
}

Image Crop::Apply(Image &&image) const {
    const size_t new_width = std::min(width_, image.Width());
    const size_t new_height = std::min(height_, image.Height());
    Image result(new_width, new_height);
    for (size_t y = 0; y < new_height; ++y) {
        for (size_t x = 0; x < new_width; ++x) {
            result.At(x, y) = image.At(x, y);
        }
    }
    return result;
}