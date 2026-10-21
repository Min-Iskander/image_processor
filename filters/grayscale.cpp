//ok
#include "grayscale.h"

namespace {
    constexpr float Redw = 0.299f;
    constexpr float Greenw = 0.587f;
    constexpr float Bluew = 0.114f;
} //namespace

void Grayscale::Transform(Color &color) const {
    const float gray = Redw * color.r + Greenw * color.g + Bluew * color.b;
    color.r = gray;
    color.g = gray;
    color.b = gray;
}