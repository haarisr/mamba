#include "window.hpp"
#include "GLFW/glfw3.h"
#include "event.hpp"
#include "glm/ext/vector_float2.hpp"
#include "input_events.hpp"
#include "window_events.hpp"

#include <iostream>

namespace mamba {

Window::Window(const WindowSpecification& spec) : m_event_handler(spec.event_handler) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_handle = glfwCreateWindow(spec.width, spec.height, spec.title.c_str(), nullptr, nullptr);
    if (!m_handle) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_handle);
    glfwSetWindowUserPointer(m_handle, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
    }

    glfwSwapInterval(1);

    glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* handle) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));

        WindowClosedEvent event;
        window->raiseEvent(event);
    });

    glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* handle, int width, int height) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
        WindowResizeEvent event((uint32_t)width, (uint32_t)height);
        window->raiseEvent(event);
    });

    glfwSetKeyCallback(m_handle, [](GLFWwindow* handle, int key, int, int action, int) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));

        switch (action) {
        case GLFW_PRESS:
        case GLFW_REPEAT: {
            KeyPressedEvent event(key, action == GLFW_REPEAT);
            window->raiseEvent(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent event(key);
            window->raiseEvent(event);
            break;
        }
        }
    });

    glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* handle, int button, int action, int) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));

        switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            window->raiseEvent(event);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            window->raiseEvent(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(m_handle, [](GLFWwindow* handle, double x_offset, double y_offset) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));

        MouseScrolledEvent event(x_offset, y_offset);
        window->raiseEvent(event);
    });

    glfwSetCursorPosCallback(m_handle, [](GLFWwindow* handle, double x, double y) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));

        MouseMovedEvent event(x, y);
        window->raiseEvent(event);
    });

    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
}

Window::~Window() {

    glfwDestroyWindow(m_handle);
    m_handle = nullptr;
    glfwTerminate();
}

bool Window::shouldClose() { return glfwWindowShouldClose(m_handle) != 0; }

void Window::update() { glfwSwapBuffers(m_handle); }

void Window::raiseEvent(Event& event) {
    if (m_event_handler)
        m_event_handler(event);
}

glm::vec2 Window::getFrameBufferSize() const {
    int width, height;
    glfwGetFramebufferSize(m_handle, &width, &height);
    return {width, height};
}

glm::vec2 Window::getMousePosition() const {
    double x, y;
    glfwGetCursorPos(m_handle, &x, &y);
    return {x, y};
}

bool Window::isKeyDown(int key) const { return glfwGetKey(m_handle, key) == GLFW_PRESS; }

bool Window::isMouseButtonDown(int button) const {
    return glfwGetMouseButton(m_handle, button) == GLFW_PRESS;
}

} // namespace mamba
