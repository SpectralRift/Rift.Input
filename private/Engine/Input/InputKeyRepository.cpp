#include <Engine/Input/InputKeyRepository.hpp>
#include <Engine/Core/Hashing/FNV.hpp>

namespace engine::input {
    InputKeyRepository& InputKeyRepository::Instance() {
        static InputKeyRepository g_InputKeyRepository;
        return g_InputKeyRepository;
    }

    std::string InputKeyRepository::GetKey(InputKeyHandle handle) {
        return m_KeyList[handle];
    }

    bool InputKeyRepository::HasKey(InputKeyHandle handle) {
        return m_KeyList.contains(handle);
    }

    InputKeyHandle InputKeyRepository::AddKey(std::string_view key) {
        // hash the key name for easier identification
        InputKeyHandle handle = engine::FNVConstHash(key);
        m_KeyList[handle] = key;
        return handle;
    }
}