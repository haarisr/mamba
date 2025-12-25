#include "button.hpp"
#include "color_layers.hpp"
#include "event.hpp"
#include "layer_stack.hpp"
#include "window.hpp"

#include <memory>
#include <ranges>

int main() {
    using namespace mamba;

    LayerStack layers;
    auto event_handler = [&layers](Event& event) {
        for (auto& layer : layers) {
            layer->onEvent(event);
            if (event.Handled)
                break;
        }
    };

    WindowSpecification spec{
        .title = "Example", .width = 800, .height = 600, .event_handler = event_handler};
    Window window(spec);
    layers.push<ButtonLayer>();
    layers.push<RedLayer>();

    while (!window.shouldClose()) {
        glfwPollEvents();

        for (auto& layer : layers | std::views::reverse) {
            layer->onUpdate(0.0);
        }

        for (auto& layer : layers | std::views::reverse) {
            layer->onRender();
        }

        layers.applyPendingTransitions();
        window.update();
    }
}
