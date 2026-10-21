//ok
#include "negative.h"

void Negative::Transform(Color &color) const {
    color.r = 1.0f - color.r;
    color.g = 1.0f - color.g;
    color.b = 1.0f - color.b;
}