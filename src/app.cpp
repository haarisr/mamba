#include "app.hpp"

#include <ranges>

namespace mamba {

App::App(const AppSpecification& spec)
    : m_window(WindowSpecification{.title = spec.title,
                                   .width = spec.width,
                                   .height = spec.height,
                                   .event_handler = [this](Event& e) { onEvent(e); }}) {}

void App::run() {
    while (!m_window.shouldClose() && m_running) {
        glfwPollEvents();

        for (auto& layer : m_layers | std::views::reverse) {
            layer->onUpdate(0.0f);
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
