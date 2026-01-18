#include "event.hpp"
#include "renderer/camera.hpp"

namespace mamba {

struct InputState {
    bool move_down{false};
    bool move_up{false};
    bool move_left{false};
    bool move_right{false};
};

class CameraController {
  public:
    CameraController(float aspect_ratio);
    void onUpdate(float dt, const InputState&);
    void onEvent(mamba::Event&);
    const OrthographicCamera& getCamera() const;
  private:
    float m_zoom{1.0f};
    float m_aspect_ratio{};
    OrthographicCamera m_camera;
    glm::vec2 m_position{};
    float m_speed{10.0f};
};

} // namespace mamba
