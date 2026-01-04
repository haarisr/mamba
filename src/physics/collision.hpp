#pragma once

#include <glm/glm.hpp>

namespace mamba::physics {

struct AABB {
    glm::vec2 position; // center
    glm::vec2 size;     // full width/height
};

/// Check if two AABBs collide
inline bool collides(const AABB& a, const AABB& b) {
    glm::vec2 half_a = a.size / 2.0f;
    glm::vec2 half_b = b.size / 2.0f;

    bool x_overlap = a.position.x - half_a.x < b.position.x + half_b.x &&
                     a.position.x + half_a.x > b.position.x - half_b.x;
    bool y_overlap = a.position.y - half_a.y < b.position.y + half_b.y &&
                     a.position.y + half_a.y > b.position.y - half_b.y;

    return x_overlap && y_overlap;
}

/// Check if point is inside AABB
inline bool pointInAABB(const glm::vec2& point, const AABB& box) {
    glm::vec2 half = box.size / 2.0f;

    return point.x >= box.position.x - half.x && point.x <= box.position.x + half.x &&
           point.y >= box.position.y - half.y && point.y <= box.position.y + half.y;
}

} // namespace mamba::physics
