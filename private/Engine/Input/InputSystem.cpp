#include <Engine/Input/InputSystem.hpp>
#include <Engine/Input/InputManager.hpp>
#include <Engine/Runtime/Logger.hpp>
#include <Engine/Core/Hashing/FNV.hpp>

namespace engine::input {
    static InputSystem *g_InputSystem;
    static runtime::Logger g_LoggerInputSystem("InputSystem");

    InputSystem *InputSystem::Instance() {
        if (!g_InputSystem) {
            g_InputSystem = new InputSystem();
        }

        return g_InputSystem;
    }

    void InputSystem::Init() {
        g_LoggerInputSystem.Log(runtime::LOG_LEVEL_DEBUG, "Initializing input system...");
        InputManager::Instance()->AddInputListener(
                std::bind(&InputSystem::InternalInputCallback, this, std::placeholders::_1));
        g_LoggerInputSystem.Log(runtime::LOG_LEVEL_INFO, "Input system initialized!");
    }

    void InputSystem::Shutdown() {
        InputManager::Instance()->RemoveInputListener(
                std::bind(&InputSystem::InternalInputCallback, this, std::placeholders::_1));
    }

    double InputSystem::GetAxis(std::string_view mapName) {
        return m_AxisState[FNVConstHash(mapName)];
    }

    bool InputSystem::GetButton(std::string_view mapName) {
        return m_ButtonState[FNVConstHash(mapName)];
    }

    bool InputSystem::InternalInputCallback(const InputEvent &event) {
        if (event.Type == InputEventType::INPUT_EVENT_TYPE_KEY_STATE_CHANGE) {
            auto kAxisBinding = m_AxisBindings.find(event.Key);
            auto kButtonBinding = m_ButtonBindings.find(event.Key);

            if (kAxisBinding != m_AxisBindings.end()) {
                m_AxisState[kAxisBinding->second.Mapping] = event.KeyState ? kAxisBinding->second.Scale : 0.0f;
                g_LoggerInputSystem.Log(runtime::LOG_LEVEL_DEBUG, "Key Input Axis %x: %f", kAxisBinding->second.Mapping,
                                        m_AxisState[kAxisBinding->second.Mapping]);

                return true;
            } else if (kButtonBinding != m_ButtonBindings.end()) {
                m_ButtonState[kButtonBinding->second] = event.KeyState;
                g_LoggerInputSystem.Log(runtime::LOG_LEVEL_DEBUG, "Key Input Button %x: %s", kButtonBinding->second,
                                        m_ButtonState[kButtonBinding->second] ? "DOWN" : "UP");

                return true;
            }
        } else if (event.Type == InputEventType::INPUT_EVENT_TYPE_AXIS_CHANGE) {
            auto axisBinding = m_AxisBindings.find(event.Axis);

            if (axisBinding != m_AxisBindings.end()) {
                m_AxisState[axisBinding->second.Mapping] = event.AxisValue * axisBinding->second.Scale;
                g_LoggerInputSystem.Log(runtime::LOG_LEVEL_DEBUG, "Input Axis %x: %f", axisBinding->second.Mapping,
                                        m_AxisState[axisBinding->second.Mapping]);

                return true;
            }
        }

        return false;
    }

    void InputSystem::Update() {
        // ToDo: implement event publishing
    }

    void InputSystem::BindAxis(std::string_view mapName, std::string_view axisOrKeyName, double scaleValue) {
        m_AxisBindings[FNVConstHash(axisOrKeyName)] = {FNVConstHash(mapName), scaleValue};
    }

    void InputSystem::UnbindAxis(std::string_view mapName, std::string_view axisOrKeyName) {
        // ToDo: implement
    }

    void InputSystem::BindButton(std::string_view mapName, std::string_view keyName) {
        m_ButtonBindings[FNVConstHash(keyName)] = FNVConstHash(mapName);
    }

    void InputSystem::UnbindButton(std::string_view mapName, std::string_view keyName) {
        // ToDo: implement
    }
}