#pragma once

#include "event.hpp"

#include <cstdint>
#include <functional>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace mamba {

struct WindowSpecification {
    std::string title;
    uint32_t width;
    uint32_t height;
    std::function<void(Event&)> event_handler;
};

class Window {

  public:
    Window(const WindowSpecification&);
    ~Window();

    bool shouldClose();
    void update();
    void raiseEvent(Event&);

    glm::vec2 getFrameBufferSize() const;
    glm::vec2 getMousePosition() const;

  private:
    GLFWwindow* m_handle;
    std::function<void(Event&)> m_event_handler;
};
} // namespace mamba
