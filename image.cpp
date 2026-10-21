//ok
#include "image.h"

Image::Image(size_t width, size_t height) : width_(width), height_(height), pixels_(width * height) {}

size_t Image::Width() const {
    return width_;
}

size_t Image::Height() const {
    return height_;
}

Color &Image::At(size_t x, size_t y) {
    return pixels_[y * width_ + x];
}

const Color &Image::At(size_t x, size_t y) const {
    return pixels_[y * width_ + x];
}