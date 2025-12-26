#pragma once

#include "event.hpp"
#include <memory>

namespace mamba {

class App;
class LayerStack;

class Layer {
    friend class LayerStack;

  public:
    Layer(App& app) : m_app(app) {}
    virtual ~Layer() = default;

    virtual void onEvent(Event&) {}
    virtual void onUpdate(float) {}
    virtual void onRender() {}

    void transitionTo(std::unique_ptr<Layer>);

    template<std::derived_from<Layer> T, typename... Args> void transitionTo(Args&&... args) {
        transitionTo(std::make_unique<T>(m_app, std::forward<Args>(args)...));
    }

  protected:
    App& m_app;

  private:
    LayerStack* m_stack = nullptr;

    void attach(LayerStack* stack) {
        m_stack = stack;
    }
};

} // namespace mamba
