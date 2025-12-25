#include "window.hpp"

int main() {
    using namespace mamba;

    WindowSpecification spec{.title="Example", .width = 800, .height = 600};
    Window window(spec);

    while (!window.shouldClose()) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.update();
    }
}
