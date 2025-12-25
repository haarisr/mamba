#include "button.hpp"
#include "color_layers.hpp"
#include "renderer/shader.hpp"

#include <print>

ButtonLayer::ButtonLayer() {
    // Load texture
    m_texture = mamba::Renderer::loadTexture("sandbox/example/textures/Button.png");

    // Create shader
    m_shader = mamba::renderer::createGraphicsShader("sandbox/example/shaders/button.vert",
                                                     "sandbox/example/shaders/button.frag");

    // Quad vertices: position (x, y) and texture coords (u, v)
    // Centered quad, aspect ratio matches texture (400x200 = 2:1)
    float aspectRatio = static_cast<float>(m_texture.width) / static_cast<float>(m_texture.height);
    float height = 0.3f;
    float width = height * aspectRatio;

    float vertices[] = {
        // positions        // texcoords
        -width, -height, 0.0f, 0.0f, // bottom left
        width,  -height, 1.0f, 0.0f, // bottom right
        width,  height,  1.0f, 1.0f, // top right
        -width, height,  0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };

    // Create VAO, VBO, EBO
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ebo);

    // Upload vertex data
    glNamedBufferStorage(m_vbo, sizeof(vertices), vertices, 0);
    glNamedBufferStorage(m_ebo, sizeof(indices), indices, 0);

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
    // glDeleteVertexArrays(1, &m_vao);
    // glDeleteBuffers(1, &m_vbo);
    // glDeleteBuffers(1, &m_ebo);
    // glDeleteProgram(m_shader);
    // glDeleteTextures(1, &m_texture.handle);
}

void ButtonLayer::onEvent(mamba::Event& event) {
    if (event.getEventType() == mamba::EventType::MouseButtonPressed) {
        std::println("Button clicked!");

        if (auto* red = getLayer<RedLayer>()) {
            red->transitionTo<GreenLayer>();
        } else if (auto* green = getLayer<GreenLayer>()) {
            green->transitionTo<BlueLayer>();
        } else if (auto* blue = getLayer<BlueLayer>()) {
            blue->transitionTo<RedLayer>();
        }

        event.Handled = true;
    }
}

void ButtonLayer::onRender() {
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader);
    glBindTextureUnit(0, m_texture.handle);
    glUniform1i(glGetUniformLocation(m_shader, "uTexture"), 0);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
