//ok
#pragma once

#include <cstddef>
#include "filter.h"

class Crop : public Filter {
public:
    Crop(size_t width, size_t height);

    Image Apply(Image &&image) const override;

private:
    size_t width_;
    size_t height_;
};