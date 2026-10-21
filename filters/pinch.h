//ok
#pragma once

#include "filter.h"

class Pinch : public Filter {
public:
    explicit Pinch(float scale);

    Image Apply(Image &&image) const override;

private:
    float scale_;
};