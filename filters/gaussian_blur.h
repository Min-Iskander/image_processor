#pragma once

#include "matrix.h"

class GaussianBlur : public MatrixFilter {
public:
    explicit GaussianBlur(float sigma);
};