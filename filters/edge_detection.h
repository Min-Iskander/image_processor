//ok
#pragma once

#include "grayscale.h"
#include "matrix.h"

class EdgeDetection : public MatrixFilter, public Grayscale {
public:
    explicit EdgeDetection(float threshold);

    Image Apply(Image &&image) const override;

private:
    float threshold_;
};
