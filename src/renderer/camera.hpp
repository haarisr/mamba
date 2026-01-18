#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mamba {

class OrthographicCamera {
  public:
    OrthographicCamera(float left, float right, float bottom, float top);
    void setProjection(float left, float right, float bottom, float top);

    void setPosition(const glm::vec2& position);
    void setRotation(float rotation);

    const glm::vec2& getPosition() const { return m_position; }
    float getRotation() const { return m_rotation; }

    const glm::mat4& getViewMatrix() const { return m_view; }
    const glm::mat4& getProjectionMatrix() const { return m_projection; }
    const glm::mat4& getViewProjectionMatrix() const { return m_viewProjection; }

  private:
    void recalculateViewMatrix();

    glm::mat4 m_view{1.0f};
    glm::mat4 m_projection{1.0f};
    glm::mat4 m_viewProjection{1.0f};

    glm::vec2 m_position{0.0f, 0.0f};
    float m_rotation{0.0f};
};

} // namespace mamba
