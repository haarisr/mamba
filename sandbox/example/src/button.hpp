#pragma once

#include "app.hpp"
#include "renderer/renderer.hpp"

#include <glad/glad.h>

class ButtonLayer : public mamba::Layer {
  public:
    ButtonLayer(mamba::App& app);
    ~ButtonLayer();

    void onEvent(mamba::Event& event) override;
    void onRender() override;

  private:
    mamba::Texture m_texture{};
    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};
    GLuint m_shader{};
};
