//ok
#pragma once

#include "../image.h"

class Filter {
public:
    virtual ~Filter() = default;

    virtual Image Apply(Image&& image) const = 0;
};