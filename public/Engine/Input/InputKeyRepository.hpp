#pragma once

#include <unordered_map>
#include <string>
#include <string_view>

namespace engine::input {
    // The Input Key Handle represents the hashed name identifier of a key.
    using InputKeyHandle = uint32_t;

    struct InputKeyRepository {
        virtual ~InputKeyRepository() = default;

        static InputKeyRepository& Instance();

        std::string GetKey(InputKeyHandle handle);
        bool HasKey(InputKeyHandle handle);
        InputKeyHandle AddKey(std::string_view keyName);
    protected:
        std::unordered_map<InputKeyHandle, std::string> m_KeyList;
    };
}