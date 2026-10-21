#include "gaussian_blur.h"
#include "../errors.h"

#include <cmath>

namespace {
    MatrixFilter::Matrix BuildMatrix(float sigma) {
        if (sigma <= 0.0f || sigma > 100.0f) {
            throw FilterParameterError("В -blur sigma должна быть больше 0 и не больше 100");
        }
        const int radius = static_cast<int>(std::ceil(3 * sigma));
        const int size = 2 * radius + 1;
        MatrixFilter::Matrix answer(size, std::vector<float>(size));
        float sum = 0.0f;
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                const float weight = std::exp(-static_cast<float>(dx * dx + dy * dy) / (2 * sigma * sigma));
                answer[dy + radius][dx + radius] = weight;
                sum += weight;
            }
        }
        for (auto &row: answer) {
            for (float &weight: row) {
                weight /= sum;
            }
        }
        return answer;
    }
}  // namespace

GaussianBlur::GaussianBlur(float sigma) : MatrixFilter(BuildMatrix(sigma)) {}