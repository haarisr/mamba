#pragma once

#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "layer.hpp"

namespace mamba {

class LayerStack {
  public:
    void push(std::unique_ptr<Layer> layer) { m_layers.push_back(std::move(layer)); }

    template <std::derived_from<Layer> Old> bool replace(std::unique_ptr<Layer> replacement) {
        for (auto [i, layer] : std::views::enumerate(m_layers)) {
            if (dynamic_cast<Old*>(layer.get())) {
                m_pending_transitions[i] = std::move(replacement);
                return true;
            }
        }
        return false;
    }

    void applyPendingTransitions() {
        for (auto& [i, replacement] : m_pending_transitions) {
            m_layers[i] = std::move(replacement);
        }
        m_pending_transitions.clear();
    }

    auto begin() { return m_layers.begin(); }
    auto end() { return m_layers.end(); }
    auto begin() const { return m_layers.begin(); }
    auto end() const { return m_layers.end(); }

    template <std::derived_from<Layer> T> T* getLayer() {
        for (auto& layer : m_layers) {
            if (auto* found = dynamic_cast<T*>(layer.get())) {
                return found;
            }
        }
        return nullptr;
    }

  private:
    std::unordered_map<size_t, std::unique_ptr<Layer>> m_pending_transitions;
    std::vector<std::unique_ptr<Layer>> m_layers;
};

} // namespace mamba
