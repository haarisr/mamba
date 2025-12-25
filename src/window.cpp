#include "window.hpp"
#include "GLFW/glfw3.h"

#include <iostream>

namespace mamba {

Window::Window(const WindowSpecification& spec) {

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

    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
}

Window::~Window() {

    glfwDestroyWindow(m_handle);
    m_handle = nullptr;
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(m_handle) != 0;
}

void Window::update() {
    glfwSwapBuffers(m_handle);
}

} // namespace mamba
