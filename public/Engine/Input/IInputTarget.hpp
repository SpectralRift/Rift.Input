#pragma once

#include <string>
#include <string_view>

namespace engine::input {
    enum InputTargetType {
        INPUT_TARGET_TYPE_TEXT,
        INPUT_TARGET_TYPE_PASSWORD,
        INPUT_TARGET_TYPE_NUMBER
    };

    struct IInputTarget {
        virtual ~IInputTarget() = default;

        virtual std::string GetText() = 0;

        virtual void SetText(std::string_view text, bool isEnter) = 0;

        virtual std::string GetHint() = 0;

        virtual InputTargetType GetTargetType() = 0;
    };
}