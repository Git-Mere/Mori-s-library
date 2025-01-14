#include "vec2.h"
#include <cmath>


float magnitude(vec2 a) noexcept
{
    return std::sqrt(magnitude_squared(a));
}

vec2 normalize(vec2 a) noexcept
{
    vec2 unit_vector(a);
    unit_vector /= magnitude(a);
    return unit_vector;
}

int magnitude(ivec2 a) noexcept
{
    return int(std::sqrt(magnitude_squared(a)));
}

ivec2 normalize(ivec2 a) noexcept
{
    ivec2 unit_vector(a);
    unit_vector /= magnitude(a);
    return unit_vector;
}