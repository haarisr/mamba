#include "app.hpp"
#include "breakout.hpp"

int main() {
    mamba::App app({.title = "Breakout", .width = 800, .height = 600});

    app.pushLayer<BreakoutLayer>();

    app.run();
}
