#include <cstdint>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace mamba {

struct WindowSpecification {
    std::string title;
    uint32_t width;
    uint32_t height;
};

class Window {

  public:
    Window(const WindowSpecification&);
    ~Window();

    bool shouldClose();
    void update();


  private:
    GLFWwindow* m_handle;
};
} // namespace mamba
