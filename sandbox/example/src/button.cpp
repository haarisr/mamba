#include "button.hpp"

#include <array>
#include <print>

#include "app.hpp"
#include "color_layers.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "renderer/shader.hpp"

ButtonLayer::ButtonLayer() {
    // Load texture
    m_texture = mamba::Renderer::loadTexture("sandbox/example/textures/Button.png");

    // Create shader
    m_shader = mamba::renderer::createGraphicsShader("sandbox/example/shaders/button.vert",
                                                     "sandbox/example/shaders/button.frag");

    std::array<float, 16> vertices = {
        // positions        // texcoords
        -0.5, -0.5, 0.0f, 0.0f, // bottom left
        0.5,  -0.5, 1.0f, 0.0f, // bottom right
        0.5,  0.5,  1.0f, 1.0f, // top right
        -0.5, 0.5,  0.0f, 1.0f  // top left
    };

    std::array<unsigned int, 6> indices{
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };

    // Create VAO, VBO, EBO
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ebo);

    // Upload vertex data
    glNamedBufferStorage(m_vbo, sizeof(vertices), vertices.data(), 0);
    glNamedBufferStorage(m_ebo, sizeof(indices), indices.data(), 0);

    // Setup VAO
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, 4 * sizeof(float));
    glVertexArrayElementBuffer(m_vao, m_ebo);

    // Position attribute (location 0)
    glEnableVertexArrayAttrib(m_vao, 0);
    glVertexArrayAttribFormat(m_vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_vao, 0, 0);

    // Texture coord attribute (location 1)
    glEnableVertexArrayAttrib(m_vao, 1);
    glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
    glVertexArrayAttribBinding(m_vao, 1, 0);
}

ButtonLayer::~ButtonLayer() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteProgram(m_shader);
    glDeleteTextures(1, &m_texture.handle);
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
    glUseProgram(m_shader);

    // Model matrix: position and scale the button
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(m_button_pos, 0.0f));
    model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(m_button_scale, 1.0f));

    // Combine view-projection with model
    glm::mat4 transform = m_camera->getViewProjectionMatrix() * model;
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(transform));

    glBindTextureUnit(0, m_texture.handle);
    glUniform1i(1, 0);

    glUniform1ui(2, m_is_hovered);

    glm::vec2 framebuffer_size = getApp()->getWindow().getFrameBufferSize();
    glViewport(0, 0, framebuffer_size.x, framebuffer_size.y);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
