//ok
#include "edge_detection.h"

#include <utility>

EdgeDetection::EdgeDetection(float threshold) : MatrixFilter({{0,  -1, 0},
                                                              {-1, 4,  -1},
                                                              {0,  -1, 0}}), threshold_(threshold) {
}

Image EdgeDetection::Apply(Image &&image) const {
    image = Grayscale::Apply(std::move(image));
    image = MatrixFilter::Apply(std::move(image));
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            Color &color = image.At(x, y);
            const float value = color.r > threshold_ ? 1.0f : 0.0f;
            color.r = value;
            color.g = value;
            color.b = value;
        }
    }
    return std::move(image);
}
