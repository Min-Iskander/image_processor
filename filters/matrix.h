//ok
#pragma once

#include <vector>

#include "filter.h"

class MatrixFilter : virtual public Filter {
public:
    using Matrix = std::vector<std::vector<float>>;

    explicit MatrixFilter(Matrix matrix);

    Image Apply(Image &&image) const override;

private:
    Matrix matrix_;
};