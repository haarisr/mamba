#pragma once

#include <glm/glm.hpp>

namespace mamba::physics {

struct AABB {
    glm::vec2 center;
    glm::vec2 size;
};

struct Circle {
    glm::vec2 center;
    float radius;
};

/// Check if two AABBs collide
inline bool collides(const AABB& a, const AABB& b) {
    glm::vec2 half_a = a.size / 2.0f;
    glm::vec2 half_b = b.size / 2.0f;

    bool x_overlap = a.center.x - half_a.x < b.center.x + half_b.x &&
                     a.center.x + half_a.x > b.center.x - half_b.x;
    bool y_overlap = a.center.y - half_a.y < b.center.y + half_b.y &&
                     a.center.y + half_a.y > b.center.y - half_b.y;

    return x_overlap && y_overlap;
}

/// Check if two Circles collide
inline bool collides(const Circle& a, const Circle& b) {
    glm::vec2 diff = a.center - b.center;
    float distance_squared = glm::dot(diff, diff);
    float radius_sum = a.radius + b.radius;
    return distance_squared < radius_sum * radius_sum;
}

/// Check if Circle and AABB collide
inline bool collides(const Circle& circle, const AABB& box) {
    glm::vec2 half = box.size / 2.0f;

    // Find closest point on box to circle center
    glm::vec2 diff = circle.center - box.center;
    glm::vec2 clamped = glm::clamp(diff, -half, half);
    glm::vec2 closest = box.center + clamped;

    // Check if distance from circle center to closest point is less than radius
    glm::vec2 distance = circle.center - closest;
    return glm::dot(distance, distance) < circle.radius * circle.radius;
}

/// Check if AABB and Circle collide (convenience overload)
inline bool collides(const AABB& box, const Circle& circle) { return collides(circle, box); }

/// Check if point is inside AABB
inline bool contains(const AABB& box, const glm::vec2& point) {
    glm::vec2 half = box.size / 2.0f;

    return point.x >= box.center.x - half.x && point.x <= box.center.x + half.x &&
           point.y >= box.center.y - half.y && point.y <= box.center.y + half.y;
}

/// Check if point is inside Circle
inline bool contains(const Circle& circle, const glm::vec2& point) {
    glm::vec2 diff = point - circle.center;
    return glm::dot(diff, diff) < circle.radius * circle.radius;
}

} // namespace mamba::physics
