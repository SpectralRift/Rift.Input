#pragma once

#include <string>

namespace engine::input {
    struct IInputDevice {
        virtual ~IInputDevice() = default;

        virtual bool Initialize() = 0;
        virtual void Destroy() = 0;
        virtual void Poll() = 0;

        virtual std::string GetName() const = 0;

        virtual int GetPlayerId() = 0;
    };
}