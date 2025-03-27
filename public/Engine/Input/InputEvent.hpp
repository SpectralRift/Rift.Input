#pragma once

#include <string>

#include <Engine/Core/Math/Vector2.hpp>
#include <Engine/Input/InputKeyRepository.hpp>

namespace engine::input {
    enum InputEventType {
        INPUT_EVENT_TYPE_UNKNOWN,

        INPUT_EVENT_TYPE_MOUSE_POSITION,
        INPUT_EVENT_TYPE_KEY_STATE_CHANGE,
        INPUT_EVENT_TYPE_INPUT_CHAR,

        INPUT_EVENT_TYPE_TOUCH_DOWN,
        INPUT_EVENT_TYPE_TOUCH_UP,
        INPUT_EVENT_TYPE_TOUCH_MOVE,
        INPUT_EVENT_TYPE_TOUCH_HOVER
    };

    struct InputEvent {
        InputEvent(InputEventType type) : Type(type), Key(0), KeyState(false), UInputChar(0), TouchFinger(0) {}
        ~InputEvent() = default;

        InputEvent(const InputEvent &other) {
            Type = other.Type;
            Key = other.Key;
            KeyState = other.KeyState;
            UInputChar = other.UInputChar;
            Position = other.Position;
            TouchFinger = other.TouchFinger;
            Position = other.Position;
        }

        InputEventType Type;
        InputKeyHandle Key;
        bool KeyState;
        uint16_t UInputChar;
        int TouchFinger;
        core::math::Vector2 Position;
    };
}
