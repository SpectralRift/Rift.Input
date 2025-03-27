#pragma once

#include <vector>
#include <mutex>
#include <string_view>
#include <functional>

#include <Engine/Core/Runtime/IThread.hpp>
#include <Engine/Core/Math/Vector2.hpp>
#include <Engine/Core/Runtime/IMutex.hpp>

#include <Engine/Input/InputEvent.hpp>

namespace engine::input {
    struct IInputDevice;

    struct InputManager {
        using InputEventDelegate = std::function<void(const InputEvent &)>;

        InputManager();

        ~InputManager();

        void Initialize();

        void Shutdown();

        void ProcessEvents();

#ifdef SHOW_PRIVATE_API
        // make sure that certain related to pushing APIs are not exposed to everyone. we wouldn't want anyone to push fake input data, would we?
        void PushKeyStateChange(InputKeyHandle key, bool newKeyState);
        void PushInputChar(uint16_t ch);
        void PushMousePosition(core::math::Vector2 position);

        // Touchscreen API
        void PushTouchMove(int fingerId, core::math::Vector2 position);
        void PushTouchUp(int fingerId, core::math::Vector2 position);
        void PushTouchDown(int fingerId, core::math::Vector2 position);
#endif

        std::vector<IInputDevice *> GetDevices();

        void RegisterDevice(IInputDevice *device);

        void UnregisterDevice(IInputDevice *device);

        void AddInputListener(InputEventDelegate listener);

        void RemoveInputListener(InputEventDelegate listener);

        static InputManager *Instance();

    protected:
        void PushEvent(InputEvent event);

        void ProcessTask();

        std::unique_ptr<core::runtime::IMutex> mtx_InputProc;

        std::vector<IInputDevice *> m_DeviceList;
        std::vector<InputEvent> m_Events;

        std::unique_ptr<core::runtime::IThread> m_Thread;
        std::vector<InputEventDelegate> m_InputDelegates;

        bool b_IsInit;
    };
}