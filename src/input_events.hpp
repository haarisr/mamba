#pragma once

#include "event.hpp"
#include "input.hpp"

#include <format>

namespace mamba {

//
// Key Events
//

class KeyEvent : public Event {
  public:
    inline Key getKeyCode() const { return m_key_code; }

  protected:
    KeyEvent(Key keycode) : m_key_code(keycode) {}

    Key m_key_code;
};

class KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(int keycode, bool is_repeat)
        : KeyEvent(static_cast<Key>(keycode)), m_is_repeat(is_repeat) {}

    inline bool isRepeat() const { return m_is_repeat; }

    std::string toString() const override {
        return std::format("KeyPressedEvent: {} (repeat={})", static_cast<int>(m_key_code),
                           m_is_repeat);
    }

    EVENT_CLASS_TYPE(KeyPressed)
  private:
    bool m_is_repeat;
};

class KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(int keycode) : KeyEvent(static_cast<Key>(keycode)) {}

    std::string toString() const override {
        return std::format("KeyReleasedEvent: {}", static_cast<int>(m_key_code));
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

//
// Mouse Events
//

class MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(double x, double y) : m_mouse_x(x), m_mouse_y(y) {}

    inline double getX() const { return m_mouse_x; }
    inline double getY() const { return m_mouse_y; }

    std::string toString() const override {
        return std::format("MouseMovedEvent: {}, {}", m_mouse_x, m_mouse_y);
    }

    EVENT_CLASS_TYPE(MouseMoved)
  private:
    double m_mouse_x, m_mouse_y;
};

class MouseScrolledEvent : public Event {
  public:
    MouseScrolledEvent(double x_offset, double y_offset)
        : m_x_offset(x_offset), m_y_offset(y_offset) {}

    inline double getXOffset() const { return m_x_offset; }
    inline double getYOffset() const { return m_y_offset; }

    std::string toString() const override {
        return std::format("MouseScrolledEvent: {}, {}", m_x_offset, m_y_offset);
    }

    EVENT_CLASS_TYPE(MouseScrolled)
  private:
    double m_x_offset, m_y_offset;
};

class MouseButtonEvent : public Event {
  public:
    inline MouseButton getMouseButton() const { return m_button; }

  protected:
    MouseButtonEvent(MouseButton button) : m_button(button) {}

    MouseButton m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(static_cast<MouseButton>(button)) {}

    std::string toString() const override {
        return std::format("MouseButtonPressedEvent: {}", static_cast<int>(m_button));
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(static_cast<MouseButton>(button)) {}

    std::string toString() const override {
        return std::format("MouseButtonReleasedEvent: {}", static_cast<int>(m_button));
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace mamba
