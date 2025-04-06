#define SHOW_PRIVATE_API

#include <Engine/Input/InputManager.hpp>
#include <Engine/Input/IInputDevice.hpp>

#include <Engine/Core/Platform.hpp>
#include <Engine/Runtime/Logger.hpp>

// define to print debugging messages when pushing events.
//#define INPUT_MANAGER_DEBUG_EVENTS

namespace engine::input {
    static InputManager *g_InputManager;
    static runtime::Logger g_LoggerInputManager("InputManager");

    InputManager::InputManager() : b_IsInit{false}, m_Thread{nullptr} {
        mtx_InputProc = core::Platform::CreateMutex();
        mtx_DeviceProc = core::Platform::CreateMutex();
    }

    InputManager::~InputManager() {
        if (mtx_InputProc) {
            mtx_InputProc = nullptr;
        }

        if (mtx_DeviceProc) {
            mtx_DeviceProc = nullptr;
        }
    }

    InputManager *InputManager::Instance() {
        if (!g_InputManager) {
            g_InputManager = new InputManager();
        }

        return g_InputManager;
    }

    std::vector<IInputDevice *> InputManager::GetDevices() {
        return m_DeviceList;
    }

    void InputManager::Initialize() {
        mtx_InputProc->Lock();

        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Initializing input manager...");
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Creating input thread...");
        m_Thread = core::Platform::CreateThread();

        if (!m_Thread) {
            g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_ERROR, "Failed to create input thread!");
            mtx_InputProc->Unlock();
            return;
        }

        m_Thread->SetName("Input Management Thread");
        m_Thread->SetTaskFunc(&InputManager::ProcessTask, this);
        m_Thread->Start();
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Created input thread!");
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_INFO, "Initialized input manager!");

        b_IsInit = true;
        mtx_InputProc->Unlock();
    }

    void InputManager::Shutdown() {
        if(!b_IsInit) return;

        mtx_InputProc->Lock();

        b_IsInit = false;

        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Shutting down the input manager...");

        if(m_Thread) {
            g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Shutting down the input thread...");

            m_Thread->Stop();
            m_Thread->Join();
            m_Thread = nullptr;
        }

        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Destroying device resources...");
        for (auto device: m_DeviceList) {
            g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Destroying resources for '%s'",
                                     device->GetName().c_str());
            device->Destroy();
        }

        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_INFO, "Destroyed device resources!");
        m_DeviceList.clear();

        mtx_InputProc->Unlock();
    }

    void InputManager::ProcessEvents() {
        // mutex to make sure that we don't have a race condition when inputs are pushed and processed
        mtx_InputProc->Lock();

        // process events
        for (auto &ev: m_Events) {
            if (core::Platform::GetVirtualKeyboard() && core::Platform::GetVirtualKeyboard()->IsVisible()) {
                switch (ev.Type) {
                    case INPUT_EVENT_TYPE_KEY_STATE_CHANGE:
                        if ((core::Platform::GetVirtualKeyboard()->GetInputIgnoreTarget() & INPUT_IGNORE_TARGET_KEY) >
                            0) {
#ifdef INPUT_MANAGER_DEBUG_EVENTS
                            g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_WARNING,
                                                     "Virtual keyboard is shown on screen; ignoring key state change event.");
#endif
                            continue;
                        }

                        break;
                    case INPUT_EVENT_TYPE_TOUCH_DOWN:
                    case INPUT_EVENT_TYPE_TOUCH_MOVE:
                        if ((core::Platform::GetVirtualKeyboard()->GetInputIgnoreTarget() & INPUT_IGNORE_TARGET_TOUCH) >
                            0) {
                            if (core::Platform::GetVirtualKeyboard()->IsVisible() &&
                                core::Platform::GetVirtualKeyboard()->IsPointOnKeyboard(ev.Position)) {
#ifdef INPUT_MANAGER_DEBUG_EVENTS
                                g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_WARNING,
                                                         "Virtual keyboard is shown on screen; ignoring touch events.");
#endif
                                continue;
                            }
                        }

                        break;
                    default:
                        break;
                }
            }

            for (const auto &inputDelegate: m_InputDelegates) {
                if(inputDelegate(ev)) {
                    g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_WARNING, "Input delegate handled the event; dismissing event.");
                    break;
                }
            }
        }

        // clear event queue
        if (!m_Events.empty()) {
            m_Events.clear();
        }

        mtx_InputProc->Unlock();
    }

    void InputManager::ProcessTask() {
        while (m_Thread->IsRunning() || b_IsInit) {
            // poll device inputs
            mtx_DeviceProc->Lock();

            for (auto d: m_DeviceList) {
                d->Poll();
            }

            mtx_DeviceProc->Unlock();
        }
    }

    void InputManager::RegisterDevice(IInputDevice *device) {
        mtx_DeviceProc->Lock();

        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_INFO, "Registering device '%s'", device->GetName().c_str());
        m_DeviceList.emplace_back(device);

        mtx_DeviceProc->Unlock();
    }

    void InputManager::UnregisterDevice(IInputDevice *device) {
        mtx_DeviceProc->Lock();

        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Unregistering device '%s'", device->GetName().c_str());

        // look for device in our list
        auto it = std::find(m_DeviceList.begin(), m_DeviceList.end(), device);

        if (it != m_DeviceList.end()) {
            m_DeviceList.erase(it);
            device->Destroy();
            g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_INFO, "Device '%s' was destroyed successfully!",
                                     device->GetName().c_str());
        } else {
            g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_WARNING,
                                     "Device '%s' was not registered in the first place!", device->GetName().c_str());
        }

        mtx_DeviceProc->Unlock();
    }

    void InputManager::PushEvent(InputEvent event) {
        mtx_InputProc->Lock();
        m_Events.emplace_back(event);
        mtx_InputProc->Unlock();
    }

    void InputManager::PushInputChar(uint16_t ch) {
        InputEvent event{INPUT_EVENT_TYPE_INPUT_CHAR};
        event.UInputChar = ch;

        PushEvent(event);
    }

    void InputManager::PushAxisChange(InputAxisHandle axis, float value) {
        InputEvent event{INPUT_EVENT_TYPE_AXIS_CHANGE};

        event.Axis = axis;
        event.AxisValue = value;

#ifdef INPUT_MANAGER_DEBUG_EVENTS
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Pushing axis change event for axis 0x%08x: %f", axis, value);
#endif

        PushEvent(event);
    }

    void InputManager::PushKeyStateChange(InputKeyHandle key, bool newKeyState) {
        if (!InputKeyRepository::Instance().HasKey(key)) {
            g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_WARNING,
                                     "Can't push key state change: key handle %08x is not part of the key registry!",
                                     key);
            return;
        }

        InputEvent event{INPUT_EVENT_TYPE_KEY_STATE_CHANGE};

        event.Key = key;
        event.KeyState = newKeyState;

#ifdef INPUT_MANAGER_DEBUG_EVENTS
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG,
                                 "Pushing key state change event: KeyHandle: 0x%08x / Name: %s / State: %s", key,
                                 InputKeyRepository::Instance().GetKey(key).c_str(), newKeyState ? "DOWN" : "UP");
#endif

        PushEvent(event);
    }

    void InputManager::PushMousePosition(core::math::Vector2 position) {
        InputEvent event{INPUT_EVENT_TYPE_MOUSE_POSITION};

        event.Position = position;

#ifdef INPUT_MANAGER_DEBUG_EVENTS
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Pushing mouse position event: %ix%i",
                                 (int) position.x, (int) position.y);
#endif

        PushEvent(event);
    }

    void InputManager::PushTouchMove(int fingerId, core::math::Vector2 position) {
        InputEvent event{INPUT_EVENT_TYPE_TOUCH_MOVE};

        event.TouchFinger = fingerId;
        event.Position = position;

#ifdef INPUT_MANAGER_DEBUG_EVENTS
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Pushing touch move event for finger %i: %ix%i",
                                 fingerId, (int) position.x, (int) position.y);
#endif

        PushEvent(event);
    }

    void InputManager::PushTouchUp(int fingerId, core::math::Vector2 position) {
        InputEvent event{INPUT_EVENT_TYPE_TOUCH_UP};

        event.TouchFinger = fingerId;
        event.Position = position;

#ifdef INPUT_MANAGER_DEBUG_EVENTS
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Pushing touch up event for finger %i: %ix%i",
                                 fingerId, (int) position.x, (int) position.y);
#endif

        PushEvent(event);
    }

    void InputManager::PushTouchDown(int fingerId, core::math::Vector2 position) {
        InputEvent event{INPUT_EVENT_TYPE_TOUCH_DOWN};

        event.TouchFinger = fingerId;
        event.Position = position;

#ifdef INPUT_MANAGER_DEBUG_EVENTS
        g_LoggerInputManager.Log(engine::runtime::LOG_LEVEL_DEBUG, "Pushing touch down event for finger %i: %ix%i",
                                 fingerId, (int) position.x, (int) position.y);
#endif

        PushEvent(event);
    }

    void InputManager::AddInputListener(InputEventDelegate listener, bool hasHighPriority) {
        mtx_InputProc->Lock();

        if(hasHighPriority) {
            m_InputDelegates.insert(m_InputDelegates.begin(), std::move(listener));
        } else {
            m_InputDelegates.push_back(std::move(listener));
        }

        mtx_InputProc->Unlock();
    }

    void InputManager::RemoveInputListener(InputEventDelegate listener) {
        mtx_InputProc->Lock();

        m_InputDelegates.erase(
                std::remove_if(m_InputDelegates.begin(), m_InputDelegates.end(),
                               [&listener](const InputEventDelegate &existingHandler) {
                                   return existingHandler.target_type() == listener.target_type() &&
                                          existingHandler.target<void(InputEvent)>() ==
                                          listener.target<void(InputEvent)>();
                               }),
                m_InputDelegates.end()
        );

        mtx_InputProc->Unlock();
    }
}