#pragma once

#include "layer.hpp"
#include "renderer/renderer.hpp"

#include <glad/glad.h>
#include <glm/ext.hpp>

class ButtonLayer : public mamba::Layer {
  public:
    ButtonLayer();
    ~ButtonLayer();

    void onUpdate(float dt) override;
    void onEvent(mamba::Event& event) override;
    void onRender() override;

  private:
    mamba::Texture m_texture{};
    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};
    GLuint m_shader{};

    // Computed per frame
    glm::vec2 m_button_pos{-0.7f, -0.7f};
    glm::vec2 m_button_scale{0.2f, 0.2f};
    bool m_is_hovered{false};
};
