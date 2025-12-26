#pragma once

#include <string>

namespace mamba {

enum class EventType {
    None = 0,
    WindowClose,
    WindowResize,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,
};

#define EVENT_CLASS_TYPE(type)                                                                     \
    static EventType getStaticType() { return EventType::type; }                                   \
    virtual EventType getEventType() const override { return getStaticType(); }                    \
    virtual const char* getName() const override { return #type; }

class Event {
  public:
    bool handled = false;

    virtual ~Event() {}
    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual std::string toString() const { return getName(); }
};

} // namespace mamba
