#pragma once

#include "layer_stack.hpp"
#include "window.hpp"

#include <string>

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

    template<std::derived_from<Layer> T, typename... Args> void pushLayer(Args&&... args) {
        m_layers.push<T>(*this, std::forward<Args>(args)...);
    }

    template<std::derived_from<Layer> T> T* getLayer() {
        return m_layers.getLayer<T>();
    }

    Window& getWindow() {
        return m_window;
    }

  private:
    void onEvent(Event& event);

    Window m_window;
    LayerStack m_layers;
    bool m_running = true;
};

} // namespace mamba
