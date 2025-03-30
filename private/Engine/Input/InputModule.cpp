#include <Engine/Input/InputModule.hpp>
#include <Engine/Input/InputSystem.hpp>
#include <Engine/Input/InputKeyRepository.hpp>

namespace engine::input {
    void InputModule::ModuleStartup() {
        auto &repoItx = InputKeyRepository::Instance();
        
        // register mouse keys
        repoItx.AddKey("Mouse_Left");
        repoItx.AddKey("Mouse_Right");
        repoItx.AddKey("Mouse_Middle");

        // register keyboard keys (letters)
        for (char c = 'A'; c <= 'Z'; ++c) {
            repoItx.AddKey(std::string("Key_") + c);
        }

        // register keyboard keys (numbers)
        for (char c = '0'; c <= '9'; ++c) {
            repoItx.AddKey(std::string("Key_") + c);
        }

        // register function keys
        for (int i = 1; i <= 24; ++i) {
            repoItx.AddKey("Key_F" + std::to_string(i));
        }

        // register modifier keys
        repoItx.AddKey("Key_LeftCtrl");
        repoItx.AddKey("Key_LeftShift");
        repoItx.AddKey("Key_LeftAlt");
        repoItx.AddKey("Key_LeftSuper");
        repoItx.AddKey("Key_RightCtrl");
        repoItx.AddKey("Key_RightShift");
        repoItx.AddKey("Key_RightAlt");
        repoItx.AddKey("Key_RightSuper");
        repoItx.AddKey("Key_Menu");

        // register punctuation and symbol keys
        repoItx.AddKey("Key_Apostrophe");
        repoItx.AddKey("Key_Comma");
        repoItx.AddKey("Key_Minus");
        repoItx.AddKey("Key_Period");
        repoItx.AddKey("Key_Slash");
        repoItx.AddKey("Key_Semicolon");
        repoItx.AddKey("Key_Equal");
        repoItx.AddKey("Key_LeftBracket");
        repoItx.AddKey("Key_Backslash");
        repoItx.AddKey("Key_RightBracket");
        repoItx.AddKey("Key_GraveAccent");

        // register system and lock keys
        repoItx.AddKey("Key_CapsLock");
        repoItx.AddKey("Key_ScrollLock");
        repoItx.AddKey("Key_NumLock");
        repoItx.AddKey("Key_PrintScreen");
        repoItx.AddKey("Key_Pause");

        // register navigation keys
        repoItx.AddKey("Key_Insert");
        repoItx.AddKey("Key_Delete");
        repoItx.AddKey("Key_Home");
        repoItx.AddKey("Key_End");
        repoItx.AddKey("Key_PageUp");
        repoItx.AddKey("Key_PageDown");

        // register arrow keys
        repoItx.AddKey("Key_ArrowUp");
        repoItx.AddKey("Key_ArrowDown");
        repoItx.AddKey("Key_ArrowLeft");
        repoItx.AddKey("Key_ArrowRight");

        // register keypad (numpad) keys
        for (int i = 0; i <= 9; ++i) {
            repoItx.AddKey("Key_Keypad" + std::to_string(i));
        }

        repoItx.AddKey("Key_KeypadDecimal");
        repoItx.AddKey("Key_KeypadDivide");
        repoItx.AddKey("Key_KeypadMultiply");
        repoItx.AddKey("Key_KeypadSubtract");
        repoItx.AddKey("Key_KeypadAdd");
        repoItx.AddKey("Key_KeypadEnter");
        repoItx.AddKey("Key_KeypadEqual");

        // register special keys
        repoItx.AddKey("Key_Space");
        repoItx.AddKey("Key_Enter");
        repoItx.AddKey("Key_Escape");
        repoItx.AddKey("Key_Backspace");
        repoItx.AddKey("Key_Tab");

        // init input system bindings
        InputSystem::Instance()->Init();

        // ToDo: remove this test binding method and use the config for that
        InputSystem::Instance()->BindAxis("MoveForward", "Key_W", 1.0);
        InputSystem::Instance()->BindAxis("MoveForward", "Key_S", -1.0);
        InputSystem::Instance()->BindAxis("MoveForward", "NX_JoyCon_LeftStickY", 1.0);

        InputSystem::Instance()->BindAxis("MoveRight", "Key_D", 1.0);
        InputSystem::Instance()->BindAxis("MoveRight", "Key_A", -1.0);
        InputSystem::Instance()->BindAxis("MoveRight", "NX_JoyCon_LeftStickX", 1.0);

        InputSystem::Instance()->BindAxis("MoveUp", "Key_E", 1.0);
        InputSystem::Instance()->BindAxis("MoveUp", "NX_JoyCon_LeftBumper", 1.0);
        InputSystem::Instance()->BindAxis("MoveUp", "Key_Q", -1.0);
        InputSystem::Instance()->BindAxis("MoveUp", "NX_JoyCon_RightBumper", -1.0);

        InputSystem::Instance()->BindButton("Jump", "Key_Space");
        InputSystem::Instance()->BindButton("Jump", "NX_JoyCon_A");

    }

    void InputModule::ModuleShutdown() {

    }
}