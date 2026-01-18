#include "breakout.hpp"

#include <algorithm>
#include <cmath>
#include <format>

#include "app.hpp"
#include "input_events.hpp"
#include "physics/collision.hpp"

// Colors for brick rows
static const glm::vec4 BRICK_COLORS[] = {
    {1.0f, 0.3f, 0.3f, 1.0f}, // Red
    {1.0f, 0.6f, 0.2f, 1.0f}, // Orange
    {1.0f, 1.0f, 0.3f, 1.0f}, // Yellow
    {0.3f, 1.0f, 0.3f, 1.0f}, // Green
    {0.3f, 0.6f, 1.0f, 1.0f}, // Blue
};

static const glm::vec4 PADDLE_COLOR{0.2f, 0.6f, 1.0f, 1.0f};
static const glm::vec4 BALL_COLOR{1.0f, 1.0f, 1.0f, 1.0f};
static const glm::vec4 BACKGROUND_COLOR{0.1f, 0.1f, 0.15f, 1.0f};

BreakoutLayer::BreakoutLayer() { m_font = mamba::Renderer::Font::create(); }

void BreakoutLayer::initGame() {
    // Reset state
    m_state = GameState::Playing;
    m_score = 0;
    m_lives = 3;
    m_bricks.clear();

    // Setup paddle at bottom center
    m_paddle.position = {m_screen_width / 2.0f, 40.0f};
    m_paddle.size = {100.0f, 20.0f};
    m_paddle.speed = 500.0f;

    // Setup ball on paddle
    resetBall();

    // Create brick grid
    const int rows = 5;
    const int cols = 10;
    const float brick_width = 70.0f;
    const float brick_height = 25.0f;
    const float padding = 5.0f;
    const float total_width = cols * (brick_width + padding) - padding;
    const float start_x = (m_screen_width - total_width) / 2.0f + brick_width / 2.0f;
    const float start_y = m_screen_height - 100.0f;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Brick brick;
            brick.position = {start_x + col * (brick_width + padding),
                              start_y - row * (brick_height + padding)};
            brick.size = {brick_width, brick_height};
            brick.color = BRICK_COLORS[row % 5];
            brick.destroyed = false;
            brick.hits = 1;
            m_bricks.push_back(brick);
        }
    }
}

void BreakoutLayer::resetBall() {
    m_ball.stuck = true;
    m_ball.radius = 10.0f;
    m_ball.velocity = {300.0f, 300.0f};
    // Ball sits on top of paddle
    m_ball.position = {m_paddle.position.x,
                       m_paddle.position.y + m_paddle.size.y / 2.0f + m_ball.radius};
}

void BreakoutLayer::onUpdate(float dt) {
    // Update screen size
    glm::vec2 fb_size = getApp()->getWindow().getFrameBufferSize();
    bool size_changed = (m_screen_width != fb_size.x || m_screen_height != fb_size.y);
    m_screen_width = fb_size.x;
    m_screen_height = fb_size.y;

    // Initialize or update camera
    if (!m_camera || size_changed) {
        m_camera = std::make_unique<mamba::OrthographicCamera>(0.0f, m_screen_width, 0.0f, m_screen_height);

        // Init game on first frame
        if (m_bricks.empty()) {
            initGame();
        }
    }

    if (m_state != GameState::Playing) {
        return;
    }

    updatePaddle(dt);
    updateBall(dt);
    checkCollisions();

    // Check win condition
    bool all_destroyed =
        std::all_of(m_bricks.begin(), m_bricks.end(), [](const Brick& b) { return b.destroyed; });
    if (all_destroyed) {
        m_state = GameState::Win;
    }
}

void BreakoutLayer::updatePaddle(float dt) {
    auto& window = getApp()->getWindow();

    float move = 0.0f;
    if (window.isKeyDown(mamba::Key::Left) || window.isKeyDown(mamba::Key::A)) {
        move -= m_paddle.speed * dt;
    }
    if (window.isKeyDown(mamba::Key::Right) || window.isKeyDown(mamba::Key::D)) {
        move += m_paddle.speed * dt;
    }

    m_paddle.position.x += move;

    // Clamp to screen bounds
    float half_width = m_paddle.size.x / 2.0f;
    m_paddle.position.x = std::clamp(m_paddle.position.x, half_width, m_screen_width - half_width);
}

void BreakoutLayer::updateBall(float dt) {
    // Move ball with paddle if stuck
    if (m_ball.stuck) {
        m_ball.position.x = m_paddle.position.x;
        return;
    }

    m_ball.position += m_ball.velocity * dt;

    // Wall collisions
    // Left wall
    if (m_ball.position.x - m_ball.radius <= 0.0f) {
        m_ball.velocity.x = std::abs(m_ball.velocity.x);
        m_ball.position.x = m_ball.radius;
    }
    // Right wall
    if (m_ball.position.x + m_ball.radius >= m_screen_width) {
        m_ball.velocity.x = -std::abs(m_ball.velocity.x);
        m_ball.position.x = m_screen_width - m_ball.radius;
    }
    // Top wall
    if (m_ball.position.y + m_ball.radius >= m_screen_height) {
        m_ball.velocity.y = -std::abs(m_ball.velocity.y);
        m_ball.position.y = m_screen_height - m_ball.radius;
    }
    // Bottom - lose life
    if (m_ball.position.y - m_ball.radius <= 0.0f) {
        m_lives--;
        if (m_lives <= 0) {
            m_state = GameState::GameOver;
        } else {
            resetBall();
        }
    }
}

void BreakoutLayer::checkCollisions() {
    using namespace mamba::physics;

    if (m_ball.stuck) {
        return;
    }

    // Ball vs Paddle collision
    Circle ball{m_ball.position, m_ball.radius};
    AABB paddle_box{m_paddle.position, m_paddle.size};

    if (auto hit = collides(ball, paddle_box)) {
        // Only bounce if ball is moving downward
        if (m_ball.velocity.y < 0.0f) {
            m_ball.velocity.y = std::abs(m_ball.velocity.y);

            // Add some angle based on where ball hit paddle
            float hit_point = m_ball.position.x - m_paddle.position.x;
            float normalized = hit_point / (m_paddle.size.x / 2.0f); // -1 to 1
            m_ball.velocity.x = normalized * 300.0f;

            // Push ball out of paddle
            m_ball.position -= hit->normal * hit->penetration;
        }
    }

    // Ball vs Bricks collision
    for (auto& brick : m_bricks) {
        if (brick.destroyed) {
            continue;
        }

        AABB brick_box{brick.position, brick.size};

        if (auto hit = collides(ball, brick_box)) {
            brick.hits--;
            if (brick.hits <= 0) {
                brick.destroyed = true;
                m_score += 10;
            }

            // Determine collision side and bounce
            if (std::abs(hit->normal.x) > std::abs(hit->normal.y)) {
                // Horizontal collision
                m_ball.velocity.x = -m_ball.velocity.x;
            } else {
                // Vertical collision
                m_ball.velocity.y = -m_ball.velocity.y;
            }

            // Push ball out of brick
            m_ball.position -= hit->normal * hit->penetration;

            break; // Only handle one brick collision per frame
        }
    }
}

void BreakoutLayer::onEvent(mamba::Event& event) {
    if (event.getEventType() == mamba::EventType::KeyPressed) {
        auto& key_event = static_cast<mamba::KeyPressedEvent&>(event);
        auto key = key_event.getKeyCode();

        if (key == mamba::Key::Space) {
            if (m_ball.stuck && m_state == GameState::Playing) {
                m_ball.stuck = false;
            }
            // Restart game if over
            if (m_state == GameState::GameOver || m_state == GameState::Win) {
                initGame();
            }
        }
        if (key == mamba::Key::Escape) {
            // Reset game
            initGame();
        }
    }
}

void BreakoutLayer::onRender() {
    auto& renderer = getApp()->getRenderer();

    // Clear background
    renderer.setClearColor(BACKGROUND_COLOR);
    renderer.clear();

    if (!m_camera) {
        return;
    }

    renderer.begin(*m_camera);

    // Draw bricks
    for (const auto& brick : m_bricks) {
        if (!brick.destroyed) {
            renderer.drawQuad(brick.position, brick.size, brick.color);
        }
    }

    // Draw paddle
    renderer.drawQuad(m_paddle.position, m_paddle.size, PADDLE_COLOR);

    // Draw ball (as a small square for now)
    // renderer.drawQuad(m_ball.position, {m_ball.radius * 2.0f, m_ball.radius * 2.0f}, BALL_COLOR);
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(m_ball.position, 0.0f));
    transform = glm::scale(transform, glm::vec3(m_ball.radius * 2.0f, m_ball.radius * 2.0f, 1.0f));
    renderer.drawCircle(transform, BALL_COLOR);

    // Draw UI text
    if (m_font) {
        // Score
        std::string score_text = std::format("Score: {}", m_score);
        renderer.drawText(score_text, *m_font, {10.0f, m_screen_height - 40.0f}, 32.0f,
                          {1.0f, 1.0f, 1.0f, 1.0f});

        // Lives
        std::string lives_text = std::format("Lives: {}", m_lives);
        renderer.drawText(lives_text, *m_font, {m_screen_width - 120.0f, m_screen_height - 40.0f},
                          32.0f, {1.0f, 1.0f, 1.0f, 1.0f});

        // Game state messages
        if (m_state == GameState::GameOver) {
            renderer.drawText("GAME OVER", *m_font,
                              {m_screen_width / 2.0f - 100.0f, m_screen_height / 2.0f}, 48.0f,
                              {1.0f, 0.3f, 0.3f, 1.0f});
            renderer.drawText("Press SPACE to restart", *m_font,
                              {m_screen_width / 2.0f - 140.0f, m_screen_height / 2.0f - 50.0f},
                              24.0f, {1.0f, 1.0f, 1.0f, 1.0f});
        } else if (m_state == GameState::Win) {
            renderer.drawText("YOU WIN!", *m_font,
                              {m_screen_width / 2.0f - 80.0f, m_screen_height / 2.0f}, 48.0f,
                              {0.3f, 1.0f, 0.3f, 1.0f});
            renderer.drawText("Press SPACE to restart", *m_font,
                              {m_screen_width / 2.0f - 140.0f, m_screen_height / 2.0f - 50.0f},
                              24.0f, {1.0f, 1.0f, 1.0f, 1.0f});
        } else if (m_ball.stuck) {
            renderer.drawText("Press SPACE to launch", *m_font,
                              {m_screen_width / 2.0f - 130.0f, m_screen_height / 2.0f - 100.0f},
                              24.0f, {1.0f, 1.0f, 1.0f, 0.7f});
        }
    }

    renderer.end();
}
