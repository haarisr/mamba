#pragma once

#include <memory>

#include <glm/ext.hpp>
#include <optional>

#include "layer.hpp"
#include "renderer/camera.hpp"
#include "renderer/index_buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/vertex_buffer.hpp"

class ButtonLayer : public mamba::Layer {
  public:
    ButtonLayer();

    void onUpdate(float dt) override;
    void onEvent(mamba::Event& event) override;
    void onRender() override;

  private:
    std::optional<mamba::Renderer::Texture> m_texture;
    std::optional<mamba::Renderer::Shader> m_shader;
    std::optional<mamba::Renderer::IndexBuffer> m_ebo;
    std::optional<mamba::Renderer::VertexBuffer> m_vbo;
    mamba::Renderer::VertexArray m_vao;

    std::unique_ptr<mamba::OrthographicCamera> m_camera;

    // Button properties in pixels
    glm::vec2 m_button_pos{100.0f, 100.0f};
    glm::vec2 m_button_scale{150.0f, 50.0f};
    bool m_is_hovered{false};
};
