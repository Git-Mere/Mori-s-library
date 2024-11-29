

#pragma once
#include "Vec2.h"
#include <utility>
#include <cstdlib>

namespace Math {
    struct [[nodiscard]] rect {
        vec2 point_1{ 0.0f, 0.0f };
        vec2 point_2{ 0.0f, 0.0f };
        float Left() const noexcept {
            return std::min(point_1.x, point_2.x);
        }
        float Right() const noexcept {
            return std::max(point_1.x, point_2.x);
        }
        float Top() const noexcept {
            return std::max(point_1.y, point_2.y);
        }
        float Bottom() const noexcept {
            return std::min(point_1.y, point_2.y);
        }

        vec2 Size() const noexcept {
            return {
                Right() - Left(),
                std::abs(Top() - Bottom())
            };
        }
    };
    struct [[nodiscard]] irect {
        ivec2 point_1{ 0, 0 };
        ivec2 point_2{ 0, 0 };
        int Left() const noexcept {
            return std::min(point_1.x, point_2.x);
        }
        int Right() const noexcept {
            return std::max(point_1.x, point_2.x);
        }
        int Top() const noexcept {
            return std::max(point_1.y, point_2.y);
        }
        int Bottom() const noexcept {
            return std::min(point_1.y, point_2.y);
        }
        ivec2 Size() const noexcept {
            return {
                Right() - Left(),
                std::abs(Top() - Bottom())
            };
        }
    };

    using Line = Math::rect;

    inline float dist(vec2 point1, vec2 point2) {
        float distX = (point2.x - point1.x) * (point2.x - point1.x);
        float distY = (point2.y - point1.y) * (point2.y - point1.y);

        return distX + distY;
    }
}

