//ok
#pragma once

#include "pixel.h"

class Grayscale : public PixelFilter {
public:
    void Transform(Color &color) const override;
};