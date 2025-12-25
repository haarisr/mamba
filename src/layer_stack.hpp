#pragma once

#include "layer.hpp"

#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>

namespace mamba {

class LayerStack {
    friend class Layer;

  public:
    template<std::derived_from<Layer> T, typename... Args> void push(Args&&... args) {
        auto layer = std::make_unique<T>(std::forward<Args>(args)...);
        layer->attach(this);
        m_layers.push_back(std::move(layer));
    }

    void applyPendingTransitions() {
        for (auto& [i, replacement] : m_pending_transitions) {
            replacement->attach(this);
            m_layers[i] = std::move(replacement);
        }
        m_pending_transitions.clear();
    }

    auto begin() {
        return m_layers.begin();
    }
    auto end() {
        return m_layers.end();
    }
    auto begin() const {
        return m_layers.begin();
    }
    auto end() const {
        return m_layers.end();
    }

    template<std::derived_from<Layer> T> T* getLayer() {
        for (auto& layer : m_layers) {
            if (auto* found = dynamic_cast<T*>(layer.get())) {
                return found;
            }
        }
        return nullptr;
    }

  private:
    void replace(Layer* current, std::unique_ptr<Layer> replacement) {
        for (auto [i, layer] : std::views::enumerate(m_layers)) {
            if (layer.get() == current) {
                m_pending_transitions[i] = std::move(replacement);
                return;
            }
        }
    }

    std::unordered_map<size_t, std::unique_ptr<Layer>> m_pending_transitions;
    std::vector<std::unique_ptr<Layer>> m_layers;
};

// Implementation of Layer::getLayer - must be after LayerStack is defined
template<std::derived_from<Layer> T> T* Layer::getLayer() {
    return m_stack ? m_stack->getLayer<T>() : nullptr;
}

} // namespace mamba
