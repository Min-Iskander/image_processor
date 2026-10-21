//ok
#include "matrix.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace {

    size_t LimitIndex(ptrdiff_t index, size_t size) {
        if (index < 0) {
            return 0;
        }
        return std::min(static_cast<size_t>(index), size - 1);
    }

    float LimitChannel(float value) {
        return std::min(1.0f, std::max(0.0f, value));
    }

} //namespace

MatrixFilter::MatrixFilter(Matrix matrix) : matrix_(std::move(matrix)) {
    if (matrix_.empty() || matrix_.size() % 2 == 0) {
        throw std::invalid_argument("Размер матрицы фильтра должен быть нечётным");
    }
    for (const auto &row: matrix_) {
        if (row.size() != matrix_.size()) {
            throw std::invalid_argument("Матрица фильтра должна быть квадратной");
        }
    }
}

Image MatrixFilter::Apply(Image &&image) const {
    const auto radius = static_cast<ptrdiff_t>(matrix_.size() / 2);
    Image result(image.Width(), image.Height());
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            for (ptrdiff_t dy = -radius; dy <= radius; ++dy) {
                for (ptrdiff_t dx = -radius; dx <= radius; ++dx) {
                    const float weight = matrix_[dy + radius][dx + radius];
                    const size_t nx = LimitIndex(static_cast<ptrdiff_t>(x) + dx, image.Width());
                    const size_t ny = LimitIndex(static_cast<ptrdiff_t>(y) + dy, image.Height());
                    const Color &neighbour = image.At(nx, ny);
                    r += weight * neighbour.r;
                    g += weight * neighbour.g;
                    b += weight * neighbour.b;
                }
            }
            Color &color = result.At(x, y);
            color.r = LimitChannel(r);
            color.g = LimitChannel(g);
            color.b = LimitChannel(b);
        }
    }
    return result;
}