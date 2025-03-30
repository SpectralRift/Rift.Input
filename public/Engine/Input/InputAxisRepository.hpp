#pragma once

#include <unordered_map>
#include <string>
#include <string_view>

namespace engine::input {
    // The Input Axis Handle represents the hashed name identifier of an axis.
    using InputAxisHandle = uint32_t;

    struct InputAxisRepository {
        virtual ~InputAxisRepository() = default;

        static InputAxisRepository& Instance();

        std::string GetAxisName(InputAxisHandle handle);
        bool HasAxis(InputAxisHandle handle);
        InputAxisHandle AddAxis(std::string_view axisName);
    protected:
        std::unordered_map<InputAxisHandle, std::string> m_AxisList;
    };
}