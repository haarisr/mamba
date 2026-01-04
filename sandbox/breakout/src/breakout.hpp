#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <glm/ext.hpp>

#include "layer.hpp"
#include "renderer/camera.hpp"
#include "renderer/font.hpp"

// Game structs - no ECS needed!
struct Paddle {
    glm::vec2 position;
    glm::vec2 size{100.0f, 20.0f};
    float speed{500.0f};
};

struct Ball {
    glm::vec2 position;
    glm::vec2 velocity{300.0f, 300.0f};
    float radius{10.0f};
    bool stuck{true}; // Stuck to paddle until launched
};

struct Brick {
    glm::vec2 position;
    glm::vec2 size{80.0f, 30.0f};
    glm::vec4 color;
    bool destroyed{false};
    int hits{1}; // Hits needed to destroy
};

enum class GameState { Playing, GameOver, Win };

class BreakoutLayer : public mamba::Layer {
  public:
    BreakoutLayer();

    void onUpdate(float dt) override;
    void onEvent(mamba::Event& event) override;
    void onRender() override;

  private:
    void initGame();
    void resetBall();
    void updatePaddle(float dt);
    void updateBall(float dt);
    void checkCollisions();
    bool checkAABB(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2,
                   const glm::vec2& size2);

    // Rendering helpers
    void drawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

    // Camera
    std::unique_ptr<mamba::OrthographicCamera> m_camera;

    // Game objects
    Paddle m_paddle;
    Ball m_ball;
    std::vector<Brick> m_bricks;

    // Game state
    GameState m_state{GameState::Playing};
    int m_score{0};
    int m_lives{3};

    // Input state (tracked via events since we're not modifying engine)
    bool m_key_left{false};
    bool m_key_right{false};
    bool m_key_space{false};

    // Font for UI
    std::optional<mamba::Renderer::Font> m_font;

    // Screen dimensions (cached)
    float m_screen_width{800.0f};
    float m_screen_height{600.0f};
};
