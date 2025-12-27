#include "camera.hpp"

namespace mamba {

OrthographicCamera::OrthographicCamera(float width, float height) { setProjection(width, height); }

void OrthographicCamera::setProjection(float width, float height) {
    m_projection = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    m_viewProjection = m_projection * m_view;
}

void OrthographicCamera::setPosition(const glm::vec2& position) {
    m_position = position;
    recalculateViewMatrix();
}

void OrthographicCamera::setRotation(float rotation) {
    m_rotation = rotation;
    recalculateViewMatrix();
}

void OrthographicCamera::recalculateViewMatrix() {
    // View matrix is the inverse of the camera's transform
    // Camera at position P, rotated by R:
    // View = inverse(translate(P) * rotate(R))
    //      = inverse(rotate(R)) * inverse(translate(P))
    //      = rotate(-R) * translate(-P)

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    m_view = glm::inverse(transform);
    m_viewProjection = m_projection * m_view;
}

} // namespace mamba
