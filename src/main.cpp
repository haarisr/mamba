#include "window.hpp"
#include <print>

int main() {
    using namespace mamba;

    auto event_handler = [](Event& event) { std::println("{}", event.toString()); };

    WindowSpecification spec{
        .title = "Example", .width = 800, .height = 600, .event_handler = event_handler};
    Window window(spec);

    while (!window.shouldClose()) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.update();
    }
}
