/*
 * Rudy Castan
 * CS200
 * Fall 2022
 */

#pragma once

struct [[nodiscard]] vec2
{
    union
    {
        struct
        {
            float x, y;
        };
        struct
        {
            float u, v;
        };
        struct
        {
            float width, height;
        };
        float elements[2];
    };

    constexpr vec2() noexcept;
    constexpr explicit vec2(float repeated_float) noexcept;
    constexpr vec2(float fx, float fy) noexcept;
    constexpr vec2 operator-();
};

constexpr vec2& operator+=(vec2& v, const vec2& adding_vector) noexcept;

constexpr vec2& operator-=(vec2& v, const vec2& adding_vector) noexcept;

constexpr vec2 operator*(const vec2& v, float scale) noexcept;

constexpr vec2 operator/(const vec2& v, float divisor) noexcept;

constexpr vec2& operator*=(vec2& v, float scale) noexcept;

constexpr vec2& operator/=(vec2& v, float divisor) noexcept;

constexpr vec2 operator+(const vec2& v1, const vec2& v2) noexcept;

constexpr vec2 operator-(const vec2& v1, const vec2& v2) noexcept;

template <typename T>
constexpr vec2 operator+(const vec2& v1, const T value) noexcept {
    return vec2(v1.x + value, v1.y + value);

};

template <typename T>
constexpr vec2 operator-(const vec2& v1, const T value) noexcept {
    return vec2(v1.x - value, v1.y - value);

};

constexpr float dot(vec2 a, vec2 b) noexcept;

constexpr vec2 perpendicular_to(vec2 a) noexcept;

constexpr float magnitude_squared(vec2 a) noexcept;

float magnitude(vec2 a) noexcept;

vec2 normalize(vec2 a) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////////////////////

constexpr vec2::vec2() noexcept : vec2(0.0f)
{
}

constexpr vec2::vec2(float repeated_float) noexcept : vec2(repeated_float, repeated_float)
{
}

constexpr vec2::vec2(float fx, float fy) noexcept : x(fx), y(fy)
{
}

constexpr vec2& operator+=(vec2& v, const vec2& adding_vector) noexcept
{
    v.x += adding_vector.x;
    v.y += adding_vector.y;
    return v;
}

constexpr vec2& operator-=(vec2& v, const vec2& adding_vector) noexcept
{
    v.x -= adding_vector.x;
    v.y -= adding_vector.y;
    return v;
}

constexpr vec2 operator*(const vec2& v, float scale) noexcept
{
    return vec2{v.x * scale, v.y * scale};
}

constexpr vec2 operator/(const vec2& v, float divisor) noexcept
{
    return vec2(v.x / divisor, v.y / divisor);
}

constexpr vec2& operator*=(vec2& v, float scale) noexcept
{
    v.x *= scale;
    v.y *= scale;
    return v;
}

constexpr vec2& operator/=(vec2& v, float divisor) noexcept
{
    v.x /= divisor;
    v.y /= divisor;
    return v;
}

constexpr vec2 operator+(const vec2& v1, const vec2& v2) noexcept
{
    return vec2{v1.x + v2.x, v1.y + v2.y};
}

constexpr vec2 operator-(const vec2& v1, const vec2& v2) noexcept
{
    return vec2{v1.x - v2.x, v1.y - v2.y};
}

constexpr bool operator==(const vec2& v1, const vec2& v2) noexcept
{
    return (v1.x == v2.x && v1.y == v2.y);
}

constexpr bool operator!=(const vec2& v1, const vec2& v2) noexcept
{
    return (v1.x != v2.x || v1.y != v2.y);
}

constexpr vec2 vec2::operator-()
{
    return { -x, -y };
}

////////////////////////////////////////////////////////
constexpr float dot(vec2 a, vec2 b) noexcept
{
    return a.x * b.x + a.y * b.y;
}

constexpr vec2 perpendicular_to(vec2 a) noexcept
{
    return vec2{a.y, -a.x};
}

constexpr float magnitude_squared(vec2 a) noexcept
{
    return dot(a, a);
}

/////////////////////////////////////////////////////////
struct [[nodiscard]] ivec2
{
    union
    {
        struct
        {
            int x, y;
        };
        struct
        {
            int u, v;
        };
        struct
        {
            int width, height;
        };
        int elements[2];
    };

    constexpr ivec2() noexcept;
    constexpr explicit ivec2(int repeated_float) noexcept;
    constexpr ivec2(int fx, int fy) noexcept;
    constexpr ivec2 operator-();
};

constexpr ivec2& operator+=(ivec2& v, const ivec2& adding_vector) noexcept;

constexpr ivec2& operator-=(ivec2& v, const ivec2& adding_vector) noexcept;

constexpr ivec2 operator*(const ivec2& v, int scale) noexcept;

constexpr ivec2 operator/(const ivec2& v, int divisor) noexcept;

constexpr ivec2& operator*=(ivec2& v, int scale) noexcept;

constexpr ivec2& operator/=(ivec2& v, int divisor) noexcept;

constexpr ivec2 operator+(const ivec2& v1, const ivec2& v2) noexcept;

constexpr ivec2 operator-(const ivec2& v1, const ivec2& v2) noexcept;

constexpr int dot(ivec2 a, ivec2 b) noexcept;

constexpr ivec2 perpendicular_to(ivec2 a) noexcept;

constexpr int magnitude_squared(ivec2 a) noexcept;

int magnitude(ivec2 a) noexcept;

ivec2 normalize(ivec2 a) noexcept;


////////////////////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////////////////////

constexpr ivec2::ivec2() noexcept : ivec2(0)
{
}

constexpr ivec2::ivec2(int repeated_float) noexcept : ivec2(repeated_float, repeated_float)
{
}

constexpr ivec2::ivec2(int fx, int fy) noexcept : x(fx), y(fy)
{
}

constexpr ivec2& operator+=(ivec2& v, const ivec2& adding_vector) noexcept
{
    v.x += adding_vector.x;
    v.y += adding_vector.y;
    return v;
}

constexpr ivec2& operator-=(ivec2& v, const ivec2& adding_vector) noexcept
{
    v.x -= adding_vector.x;
    v.y -= adding_vector.y;
    return v;
}

constexpr ivec2 operator*(const ivec2& v, int scale) noexcept
{
    return ivec2{ v.x * scale, v.y * scale };
}

constexpr ivec2 operator/(const ivec2& v, int divisor) noexcept
{
    return ivec2(v.x / divisor, v.y / divisor);
}

constexpr ivec2& operator*=(ivec2& v, int scale) noexcept
{
    v.x *= scale;
    v.y *= scale;
    return v;
}

constexpr ivec2& operator/=(ivec2& v, int divisor) noexcept
{
    v.x /= divisor;
    v.y /= divisor;
    return v;
}

constexpr ivec2 operator+(const ivec2& v1, const ivec2& v2) noexcept
{
    return ivec2{ v1.x + v2.x, v1.y + v2.y };
}

constexpr ivec2 operator-(const ivec2& v1, const ivec2& v2) noexcept
{
    return ivec2{ v1.x - v2.x, v1.y - v2.y };
}

constexpr bool operator==(const ivec2& v1, const ivec2& v2) noexcept
{
    return (v1.x == v2.x && v1.y == v2.y);
}

constexpr bool operator!=(const ivec2& v1, const ivec2& v2) noexcept
{
    return (v1.x != v2.x || v1.y != v2.y);
}

constexpr ivec2 ivec2::operator-()
{
    return { -x, -y };
}

////////////////////////////////////////////////////////
constexpr int dot(ivec2 a, ivec2 b) noexcept
{
    return a.x * b.x + a.y * b.y;
}

constexpr ivec2 perpendicular_to(ivec2 a) noexcept
{
    return ivec2{ a.y, -a.x };
}

constexpr int magnitude_squared(ivec2 a) noexcept
{
    return dot(a, a);
}