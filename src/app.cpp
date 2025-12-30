#include "app.hpp"

#include <GLFW/glfw3.h>
#include <ranges>

namespace mamba {

App::App(const AppSpecification& spec)
    : m_window(WindowSpecification{.title = spec.title,
                                   .width = spec.width,
                                   .height = spec.height,
                                   .event_handler = [this](Event& e) { onEvent(e); }}) {}

void App::run() {
    float last_time = static_cast<float>(glfwGetTime());

    while (!m_window.shouldClose() && m_running) {
        float current_time = static_cast<float>(glfwGetTime());
        float dt = current_time - last_time;
        last_time = current_time;

        glfwPollEvents();

        for (auto& layer : m_layers | std::views::reverse) {
            layer->onUpdate(dt);
        }

        for (auto& layer : m_layers | std::views::reverse) {
            layer->onRender();
        }

        m_layers.applyPendingTransitions();
        m_window.update();
    }
}

void App::onEvent(Event& event) {
    for (auto& layer : m_layers) {
        layer->onEvent(event);
        if (event.handled)
            break;
    }
}

} // namespace mamba
