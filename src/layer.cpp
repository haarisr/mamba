#include "layer.hpp"
#include "layer_stack.hpp"

namespace mamba {

void Layer::transitionTo(std::unique_ptr<Layer> layer) {
    m_stack->replace(this, std::move(layer));
}

} // namespace mamba
