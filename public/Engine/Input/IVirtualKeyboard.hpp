#pragma once

#include <string>
#include <Engine/Core/Math/Vector2.hpp>

namespace engine::input {
    struct IInputTarget;

    enum InputIgnoreTarget {
        INPUT_IGNORE_TARGET_NONE,
        INPUT_IGNORE_TARGET_TOUCH = 1 << 0,
        INPUT_IGNORE_TARGET_KEY = 1 << 1,
        INPUT_IGNORE_TARGET_ALL = INPUT_IGNORE_TARGET_TOUCH | INPUT_IGNORE_TARGET_KEY
    };

    struct IVirtualKeyboard {
        virtual ~IVirtualKeyboard() = default;

        virtual bool Initialize() = 0;
        virtual void Destroy() = 0;
        virtual void Update() = 0;

        virtual void Show(bool show, IInputTarget* target) = 0;
        virtual bool IsVisible() = 0;

        virtual bool IsPointOnKeyboard(const core::math::Vector2& point) = 0;

        // specify input targets that must be ignored while the virtual input device is shown
        virtual InputIgnoreTarget GetInputIgnoreTarget() const = 0;
    };
}