//ok
#pragma once

#include "pixel.h"

class Negative : public PixelFilter {
public:
    void Transform(Color &color) const override;
};