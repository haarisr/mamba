#include "camera_controller.hpp"
#include "input_events.hpp"
#include "window_events.hpp"

namespace mamba {

CameraController::CameraController(float aspect_ratio)
    : m_aspect_ratio(aspect_ratio),
      m_camera(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom) {}

void CameraController::onUpdate(float dt, const InputState& state) {
    glm::vec2 direction{};
    if (state.move_down)
        direction.y -= dt * m_speed;
    if (state.move_up)
        direction.y += dt * m_speed;
    if (state.move_left)
        direction.x -= dt * m_speed;
    if (state.move_right)
        direction.x += dt * m_speed;

    if (direction.x != 0.0f || direction.y != 0.0f) {
        m_position += direction;
        m_camera.setPosition(m_position);
    }
}

void CameraController::onEvent(mamba::Event& event) {
    if (event.getEventType() == EventType::WindowResize) {
        auto& resize_event = static_cast<WindowResizeEvent&>(event);
        auto width = static_cast<float>(resize_event.getWidth());
        auto height = static_cast<float>(resize_event.getHeight());
        m_aspect_ratio = width / height;
        m_camera.setProjection(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
    }

    if (event.getEventType() == EventType::MouseScrolled) {
        auto& scroll_event = static_cast<MouseScrolledEvent&>(event);
        m_zoom = m_zoom - static_cast<float>(scroll_event.getYOffset()) * 0.25f;
        m_zoom = std::max(m_zoom, 0.25f);
        m_camera.setProjection(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
    }
}

const OrthographicCamera& CameraController::getCamera() const { return m_camera; }

} // namespace mamba
