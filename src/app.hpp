#pragma once

#include <memory>
#include <string>
#include <utility>

#include "layer_stack.hpp"
#include "renderer/renderer.hpp"
#include "window.hpp"

namespace mamba {

struct AppSpecification {
    std::string title;
    uint32_t width;
    uint32_t height;
};

class App {
  public:
    App(const AppSpecification& spec);
    virtual ~App() = default;

    void run();

    template <std::derived_from<Layer> T, typename... Args> void pushLayer(Args&&... args) {
        auto layer = std::make_unique<T>(std::forward<Args>(args)...);
        layer->attach(this);
        m_layers.push(std::move(layer));
    }

    template <std::derived_from<Layer> T> T* getLayer() { return m_layers.getLayer<T>(); }

    template <std::derived_from<Layer> Old, std::derived_from<Layer> New, typename... Args>
    bool replaceLayer(Args&&... args) {
        auto replacement = std::make_unique<New>(std::forward<Args>(args)...);
        replacement->attach(this);
        return m_layers.replace<Old>(std::move(replacement));
    }

    Window& getWindow() { return m_window; }
    Renderer::Renderer2D& getRenderer() { return m_renderer; }

  private:
    void onEvent(Event& event);

    Window m_window;
    Renderer::Renderer2D m_renderer;
    LayerStack m_layers;
    bool m_running = true;
};

} // namespace mamba
