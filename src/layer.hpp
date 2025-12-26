#pragma once

#include "event.hpp"

namespace mamba {

class App;

class Layer {
    friend class App;

  public:
    virtual ~Layer() = default;

    virtual void onEvent(Event&) {}
    virtual void onUpdate(float) {}
    virtual void onRender() {}

    App* getApp() { return m_app; }

  private:
    App* m_app = nullptr;

    void attach(App* app) { m_app = app; }
};

} // namespace mamba
