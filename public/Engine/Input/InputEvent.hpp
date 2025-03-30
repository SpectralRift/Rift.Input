#pragma once

#include <string>

#include <Engine/Core/Math/Vector2.hpp>

#include <Engine/Input/InputKeyRepository.hpp>
#include <Engine/Input/InputAxisRepository.hpp>

namespace engine::input {
    enum InputEventType {
        INPUT_EVENT_TYPE_UNKNOWN,

        INPUT_EVENT_TYPE_MOUSE_POSITION,
        INPUT_EVENT_TYPE_AXIS_CHANGE,
        INPUT_EVENT_TYPE_KEY_STATE_CHANGE,
        INPUT_EVENT_TYPE_INPUT_CHAR,

        INPUT_EVENT_TYPE_TOUCH_DOWN,
        INPUT_EVENT_TYPE_TOUCH_UP,
        INPUT_EVENT_TYPE_TOUCH_MOVE,
        INPUT_EVENT_TYPE_TOUCH_HOVER
    };

    // ToDo: improve storage size by using unions
    struct InputEvent {
        InputEvent(InputEventType type) : Type(type), AxisValue(0.0f) {}

        ~InputEvent() = default;

        InputEvent(const InputEvent &other) {
            Type = other.Type;

            Key = other.Key;
            Axis = other.Axis;

            KeyState = other.KeyState;
            UInputChar = other.UInputChar;
            TouchFinger = other.TouchFinger;
            AxisValue = other.AxisValue;
            Position = other.Position;
        }

        InputEventType Type;

        union {
            InputKeyHandle Key;
            InputAxisHandle Axis;
        };

        bool KeyState;
        uint16_t UInputChar;
        int TouchFinger;
        float AxisValue;
        core::math::Vector2 Position;
    };
}
