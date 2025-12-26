#include "button.hpp"
#include "color_layers.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer/shader.hpp"
#include "window.hpp"

#include <array>
#include <print>

ButtonLayer::ButtonLayer(mamba::App& app) : Layer(app) {
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
    glm::vec2 framebuffer_size = m_app.getWindow().getFrameBufferSize();
    float aspect_ratio = framebuffer_size.x / framebuffer_size.y;

    // Update button scale for current aspect ratio
    float button_size = 0.2f;
    m_button_scale = glm::vec2(button_size / aspect_ratio, button_size);

    // Check hover state
    glm::vec2 mouse_pos = m_app.getWindow().getMousePosition();
    glm::vec2 mouse_ndc = (mouse_pos / framebuffer_size) * 2.0f - 1.0f;
    mouse_ndc.y *= -1.0f;

    glm::vec2 half_size = m_button_scale * 0.5f;
    glm::vec2 min_bound = m_button_pos - half_size;
    glm::vec2 max_bound = m_button_pos + half_size;

    m_is_hovered = mouse_ndc.x >= min_bound.x && mouse_ndc.x <= max_bound.x &&
                   mouse_ndc.y >= min_bound.y && mouse_ndc.y <= max_bound.y;
}

void ButtonLayer::onEvent(mamba::Event& event) {
    if (event.getEventType() == mamba::EventType::MouseButtonPressed) {
        if (!m_is_hovered)
            return;
        std::println("Button clicked!");
        if (auto* red = m_app.getLayer<RedLayer>()) {
            red->transitionTo<GreenLayer>();
        } else if (auto* green = m_app.getLayer<GreenLayer>()) {
            green->transitionTo<BlueLayer>();
        } else if (auto* blue = m_app.getLayer<BlueLayer>()) {
            blue->transitionTo<RedLayer>();
        }

        event.handled = true;
    }
}

void ButtonLayer::onRender() {
    glUseProgram(m_shader);

    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(m_button_pos, 0.0f));
    transform = glm::scale(transform, glm::vec3(m_button_scale, 1.0f));

    auto transform_loc = glGetUniformLocation(m_shader, "uTransform");
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindTextureUnit(0, m_texture.handle);
    glUniform1i(glGetUniformLocation(m_shader, "uTexture"), 0);

    glm::vec2 framebuffer_size = m_app.getWindow().getFrameBufferSize();
    glViewport(0, 0, framebuffer_size.x, framebuffer_size.y);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
