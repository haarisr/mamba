#pragma once

#include <memory>

#include <glm/ext.hpp>

#include "layer.hpp"
#include "renderer/camera.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"

class ButtonLayer : public mamba::Layer {
  public:
    ButtonLayer();
    ~ButtonLayer();

    void onUpdate(float dt) override;
    void onEvent(mamba::Event& event) override;
    void onRender() override;

  private:
    mamba::Texture m_texture{};
    std::optional<mamba::Renderer::Shader> m_shader;
    mamba::Renderer::VertexArray m_vao;
    GLuint m_vbo{};
    GLuint m_ebo{};


    std::unique_ptr<mamba::OrthographicCamera> m_camera;

    // Button properties in pixels
    glm::vec2 m_button_pos{100.0f, 100.0f};
    glm::vec2 m_button_scale{150.0f, 50.0f};
    bool m_is_hovered{false};
};
