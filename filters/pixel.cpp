//ok
#include "pixel.h"

#include <utility>

Image PixelFilter::Apply(Image &&image) const {
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            Color &color = image.At(x, y);
            Transform(color);
        }
    }
    return std::move(image);
}
