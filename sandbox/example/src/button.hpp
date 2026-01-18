#pragma once

#include <glm/ext.hpp>
#include <optional>

#include "layer.hpp"
#include "renderer/camera_controller.hpp"
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
    std::optional<mamba::CameraController> m_camera_controller;
    std::optional<mamba::OrthographicCamera> m_ui_camera;
    std::optional<mamba::Renderer::Texture> m_texture;
    std::optional<mamba::Renderer::Font> m_font;

    // UI button in pixel space
    glm::vec2 m_button_pos{0.0f, 0.0f};      // center (px)
    glm::vec2 m_button_scale{220.0f, 80.0f}; // size (px)
    glm::vec2 m_button_padding{24.0f, 24.0f};
    bool m_is_hovered{false};

    Ball m_ball;
};
