#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <optional>

namespace mamba::physics {

struct AABB {
    glm::vec2 center;
    glm::vec2 size;
};

struct Circle {
    glm::vec2 center;
    float radius;
};

/// Detailed collision information
struct CollisionInfo {
    glm::vec2 normal;
    float penetration; // How far to push along normal
};

/// Check if two AABBs overlap (fast boolean check)
inline bool overlaps(const AABB& a, const AABB& b) {
    glm::vec2 half_a = a.size / 2.0f;
    glm::vec2 half_b = b.size / 2.0f;

    bool x_overlap = a.center.x - half_a.x < b.center.x + half_b.x &&
                     a.center.x + half_a.x > b.center.x - half_b.x;
    bool y_overlap = a.center.y - half_a.y < b.center.y + half_b.y &&
                     a.center.y + half_a.y > b.center.y - half_b.y;

    return x_overlap && y_overlap;
}

/// Check if two Circles overlap (fast boolean check)
inline bool overlaps(const Circle& a, const Circle& b) {
    glm::vec2 diff = a.center - b.center;
    float distance_squared = glm::dot(diff, diff);
    float radius_sum = a.radius + b.radius;
    return distance_squared < radius_sum * radius_sum;
}

/// Check if Circle and AABB overlap (fast boolean check)
inline bool overlaps(const Circle& circle, const AABB& box) {
    glm::vec2 half = box.size / 2.0f;

    // Find closest point on box to circle center
    glm::vec2 diff = circle.center - box.center;
    glm::vec2 clamped = glm::clamp(diff, -half, half);
    glm::vec2 closest = box.center + clamped;

    // Check if distance from circle center to closest point is less than radius
    glm::vec2 distance = circle.center - closest;
    return glm::dot(distance, distance) < circle.radius * circle.radius;
}

/// Check if AABB and Circle overlap (convenience overload)
inline bool overlaps(const AABB& box, const Circle& circle) { return overlaps(circle, box); }

/// Get collision info between Circle and AABB
/// Returns std::nullopt if no collision, otherwise returns normal and penetration
/// Normal points from Circle toward AABB
inline std::optional<CollisionInfo> collides(const Circle& circle, const AABB& box) {
    glm::vec2 half = box.size / 2.0f;

    // Find closest point on box to circle center
    glm::vec2 diff = circle.center - box.center;
    glm::vec2 clamped = glm::clamp(diff, -half, half);
    glm::vec2 closest = box.center + clamped;

    // Vector from closest point to circle center
    glm::vec2 to_circle = circle.center - closest;
    float distance_sq = glm::dot(to_circle, to_circle);

    if (distance_sq > circle.radius * circle.radius) {
        return std::nullopt;
    }

    CollisionInfo info;
    float distance = std::sqrt(distance_sq);

    if (distance > 0.0001f) {
        // Circle center is outside box - contact normal (radial)
        info.normal = -to_circle / distance;
        info.penetration = circle.radius - distance;
    } else {
        // Circle center is inside box - find axis of minimum penetration
        glm::vec2 pen = half - abs(diff);

        if (pen.x < pen.y) {
            info.normal = {diff.x > 0 ? -1.0f : 1.0f, 0.0f};
            info.penetration = pen.x + circle.radius;
        } else {
            info.normal = {0.0f, diff.y > 0 ? -1.0f : 1.0f};
            info.penetration = pen.y + circle.radius;
        }
    }

    return info;
}

/// Get collision info between AABB and Circle (convenience overload)
/// Normal points from AABB toward Circle
inline std::optional<CollisionInfo> collides(const AABB& box, const Circle& circle) {
    auto result = collides(circle, box).transform([](CollisionInfo&& value) -> CollisionInfo {
        return {-value.normal, value.penetration};
    });

    return result;
}

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
