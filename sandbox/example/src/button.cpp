#include "button.hpp"

#include "app.hpp"
#include "color_layers.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"
#include <print>

ButtonLayer::ButtonLayer() {
    using namespace ::mamba::Renderer;
    // Load texture
    m_texture = Texture::create("sandbox/example/textures/Button.png");
}

void ButtonLayer::onUpdate(float /*dt*/) {
    glm::vec2 framebuffer_size = getApp()->getWindow().getFrameBufferSize();

    // Initialize or update camera projection on resize
    if (!m_camera) {
        m_camera =
            std::make_unique<mamba::OrthographicCamera>(framebuffer_size.x, framebuffer_size.y);
    } else {
        m_camera->setProjection(framebuffer_size.x, framebuffer_size.y);
    }

    // Check hover state using pixel coordinates
    // Mouse position is in pixels with (0,0) at top-left, Y down
    // Our camera has (0,0) at bottom-left, Y up
    glm::vec2 mouse_pos = getApp()->getWindow().getMousePosition();
    mouse_pos.y = framebuffer_size.y - mouse_pos.y; // Flip Y to match camera

    glm::vec2 half_size = m_button_scale * 0.5f;
    glm::vec2 min_bound = m_button_pos - half_size;
    glm::vec2 max_bound = m_button_pos + half_size;

    m_is_hovered = mouse_pos.x >= min_bound.x && mouse_pos.x <= max_bound.x &&
                   mouse_pos.y >= min_bound.y && mouse_pos.y <= max_bound.y;
}

void ButtonLayer::onEvent(mamba::Event& event) {
    if (event.getEventType() == mamba::EventType::MouseButtonPressed) {
        if (!m_is_hovered)
            return;
        bool success = getApp()->replaceLayer<RedLayer, GreenLayer>();
        if (!success)
            success = getApp()->replaceLayer<GreenLayer, BlueLayer>();
        if (!success)
            getApp()->replaceLayer<BlueLayer, RedLayer>();

        event.handled = true;
    }
}

void ButtonLayer::onRender() {

    // Model matrix: position and scale the button
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(m_button_pos, 0.0f));
    model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(m_button_scale, 1.0f));

    auto& renderer = getApp()->getRenderer();

    glm::vec2 framebuffer_size = getApp()->getWindow().getFrameBufferSize();
    glViewport(0, 0, static_cast<int>(framebuffer_size.x), static_cast<int>(framebuffer_size.y));

    renderer.begin(*m_camera);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto tint = m_is_hovered ? glm::vec4(2.0, 2.0, 2.0, 1.0) : glm::vec4(1.0, 1.0, 1.0, 1.0);
    renderer.drawQuad(model, m_texture.value(), tint);
}
