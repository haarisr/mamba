#include "button.hpp"

#include "app.hpp"
#include "color_layers.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "input.hpp"
#include "physics/collision.hpp"

ButtonLayer::ButtonLayer() {
    using namespace ::mamba::Renderer;
    // Load texture
    m_texture = Texture::create("sandbox/example/assets/textures/Button.png");
    m_font = Font::create();
}

void ButtonLayer::onUpdate(float dt) {
    glm::vec2 framebuffer_size = getApp()->getWindow().getFrameBufferSize();

    // Initialize or update camera projection on resize
    if (!m_camera_controller) {
        float aspect_ratio = framebuffer_size.x / framebuffer_size.y;
        m_camera_controller = mamba::CameraController(aspect_ratio);
        m_ball = Ball({0.0f, 0.0f}, {0, 0}, 0.25, 1);
        m_ui_camera = mamba::OrthographicCamera(0.0f, framebuffer_size.x, 0.0f, framebuffer_size.y);
    }

    mamba::InputState state;
    const auto& window = getApp()->getWindow();
    state.move_up = window.isKeyDown(mamba::Key::W);
    state.move_left = window.isKeyDown(mamba::Key::A);
    state.move_down = window.isKeyDown(mamba::Key::S);
    state.move_right = window.isKeyDown(mamba::Key::D);
    m_camera_controller->onUpdate(dt, state);

    m_ui_camera->setProjection(0.0f, framebuffer_size.x, 0.0f, framebuffer_size.y);

    // Anchor button to lower-left in pixels
    glm::vec2 half_size = m_button_scale * 0.5f;
    m_button_pos = m_button_padding + half_size;

    // Hover in pixel space (mouse is top-left origin -> convert to bottom-left)
    glm::vec2 mouse_pos = getApp()->getWindow().getMousePosition();
    mouse_pos.y = framebuffer_size.y - mouse_pos.y;
    glm::vec2 min_bound = m_button_pos - half_size;
    glm::vec2 max_bound = m_button_pos + half_size;

    m_is_hovered = mouse_pos.x >= min_bound.x && mouse_pos.x <= max_bound.x &&
                   mouse_pos.y >= min_bound.y && mouse_pos.y <= max_bound.y;

    m_ball.velocity.y += -9.8f * dt;
    m_ball.position.y += m_ball.velocity.y * dt;

    mamba::physics::Circle ball_collider = {m_ball.position, m_ball.radius};
    mamba::physics::AABB ground_collider = {{0.0f, -1.0f}, {1.0f, 0.001f}};
    // Bottom wall
    if (auto hit = mamba::physics::collides(ball_collider, ground_collider)) {
        m_ball.position.y += hit->penetration;

        glm::vec2 normal = -hit->normal; // Ground to ball
        float velocity_along_normal = glm::dot(m_ball.velocity, normal);
        if (velocity_along_normal < 0) {
            float restitution = 1.0f;
            float impulse = -(1.0f + restitution) * velocity_along_normal;
            m_ball.velocity += impulse * normal;
            if (std::abs(m_ball.velocity.y) < 0.05f) {
                m_ball.velocity.y = 0.0f;
            }
        }
    }
}

void ButtonLayer::onEvent(mamba::Event& event) {
    if (m_camera_controller) {
        m_camera_controller->onEvent(event);
    }

    if (event.getEventType() == mamba::EventType::MouseButtonPressed) {
        if (!m_is_hovered)
            return;
        bool success = getApp()->replaceLayer<RedLayer, GreenLayer>();
        if (!success)
            success = getApp()->replaceLayer<GreenLayer, BlueLayer>();
        if (!success)
            getApp()->replaceLayer<BlueLayer, RedLayer>();

        event.handled = true;
    }
}

void ButtonLayer::onRender() {
    auto& renderer = getApp()->getRenderer();

    // World pass
    renderer.begin(m_camera_controller->getCamera());

    glm::mat4 circle_model(1.0f);
    circle_model = glm::translate(circle_model, glm::vec3(m_ball.position, 1.0));
    circle_model = glm::scale(circle_model, glm::vec3(glm::vec2(m_ball.radius * 2.0f), 1.0f));
    renderer.drawCircle(circle_model, {1.0, 1.0, 1.0, 1.0});

    renderer.drawText("Hello Mamba!", *m_font, {-1.33f, 1.0f}, 0.3f, {1.0f, 1.0f, 1.0f, 1.0f});

    renderer.end();

    // UI pass
    renderer.begin(*m_ui_camera);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(m_button_pos, 0.0f));
    model = glm::scale(model, glm::vec3(m_button_scale, 1.0f));
    auto tint = m_is_hovered ? glm::vec4(2.0, 2.0, 2.0, 1.0) : glm::vec4(1.0, 1.0, 1.0, 1.0);
    renderer.drawQuad(model, m_texture.value(), tint);

    renderer.end();
}
