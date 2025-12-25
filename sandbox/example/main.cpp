#include "event.hpp"
#include "layer_stack.hpp"
#include "window.hpp"

#include <memory>
#include <print>

class RedLayer;
class BlueLayer;

class RedLayer : public mamba::Layer {
  public:
    void onEvent(mamba::Event& event) override;
    void onRender() override {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

class GreenLayer : public mamba::Layer {
  public:
    void onEvent(mamba::Event& event) override;
    void onRender() override {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

class BlueLayer : public mamba::Layer {
  public:
    void onEvent(mamba::Event& event) override;
    void onRender() override {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

void RedLayer::onEvent(mamba::Event& event) {
    if (event.getEventType() == mamba::EventType::MouseButtonPressed) {
        std::println("transitioning to green");
        transitionTo<GreenLayer>();
    }
}

void GreenLayer::onEvent(mamba::Event& event) {
    if (event.getEventType() == mamba::EventType::MouseButtonPressed) {
        std::println("transitioning to blue");
        transitionTo<BlueLayer>();
    }
}

void BlueLayer::onEvent(mamba::Event& event) {
    if (event.getEventType() == mamba::EventType::MouseButtonPressed) {
        std::println("transitioning to red");
        transitionTo<RedLayer>();
    }
}

int main() {
    using namespace mamba;

    LayerStack layers;
    auto event_handler = [&layers](Event& event) {
        for (auto& layer : layers) {
            layer->onEvent(event);
        }
    };

    WindowSpecification spec{
        .title = "Example", .width = 800, .height = 600, .event_handler = event_handler};
    Window window(spec);
    layers.push<RedLayer>();

    while (!window.shouldClose()) {
        glfwPollEvents();

        for (auto& layer : layers) {
            layer->onUpdate(0.0);
        }

        for (auto& layer : layers) {
            layer->onRender();
        }

        layers.applyPendingTransitions();
        window.update();
    }
}
