#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include <Engine/Input/InputEvent.hpp>

namespace engine::input {
    using InputMapHandle = uint32_t;

    struct AxisInputBinding {
        InputMapHandle Mapping;
        double Scale;

        AxisInputBinding() : Mapping(0), Scale(0) {}
        AxisInputBinding(InputMapHandle mapping, double scale) : Mapping(mapping), Scale(scale) {}
        AxisInputBinding(const AxisInputBinding &other) : Mapping(other.Mapping), Scale(other.Scale) {}
    };

    struct InputSystem {
        InputSystem() = default;

        virtual ~InputSystem() = default;

        void Init();

        void Shutdown();

        void Update();

        double GetAxis(std::string_view mapName);

        void BindAxis(std::string_view mapName, std::string_view axisOrKeyName, double scaleValue);

        void UnbindAxis(std::string_view mapName, std::string_view axisOrKeyName);

        bool GetButton(std::string_view mapName);

        void BindButton(std::string_view mapName, std::string_view keyName);

        void UnbindButton(std::string_view mapName, std::string_view keyName);

        static InputSystem *Instance();

    protected:
        bool InternalInputCallback(const InputEvent &event);

        std::unordered_map<InputAxisHandle, AxisInputBinding> m_AxisBindings;
        std::unordered_map<InputKeyHandle, InputMapHandle> m_ButtonBindings;

        std::unordered_map<InputMapHandle, double> m_AxisState;
        std::unordered_map<InputMapHandle, bool> m_ButtonState;
    };
}