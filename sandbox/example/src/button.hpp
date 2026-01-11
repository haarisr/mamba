#pragma once

#include <memory>

#include <glm/ext.hpp>
#include <optional>

#include "layer.hpp"
#include "renderer/camera.hpp"
#include "renderer/font.hpp"
#include "renderer/texture.hpp"

struct Ball {
    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    float mass;
};

class ButtonLayer : public mamba::Layer {
  public:
    ButtonLayer();

    void onUpdate(float dt) override;
    void onEvent(mamba::Event& event) override;
    void onRender() override;

  private:
    std::optional<mamba::Renderer::Texture> m_texture;
    std::optional<mamba::Renderer::Font> m_font;
    std::unique_ptr<mamba::OrthographicCamera> m_camera;

    // Button properties in pixels
    glm::vec2 m_button_pos{100.0f, 100.0f};
    glm::vec2 m_button_scale{150.0f, 50.0f};
    bool m_is_hovered{false};

    Ball m_ball;
};
