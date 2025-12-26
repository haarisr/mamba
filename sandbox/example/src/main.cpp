#include "app.hpp"
#include "button.hpp"
#include "color_layers.hpp"

int main() {
    mamba::App app({.title = "Example", .width = 800, .height = 600});

    app.pushLayer<ButtonLayer>();
    app.pushLayer<RedLayer>();

    app.run();
}
