//ok
#pragma once

#include "filter.h"

class PixelFilter : virtual public Filter {
public:
    Image Apply(Image &&image) const override;

    virtual void Transform(Color &color) const = 0;
};