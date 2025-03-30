#include <Engine/Input/InputAxisRepository.hpp>
#include <Engine/Core/Hashing/FNV.hpp>

namespace engine::input {
    InputAxisRepository& InputAxisRepository::Instance() {
        static InputAxisRepository g_InputAxisRepository;
        return g_InputAxisRepository;
    }

    std::string InputAxisRepository::GetAxisName(InputAxisHandle handle) {
        return m_AxisList.at(handle);
    }

    bool InputAxisRepository::HasAxis(InputAxisHandle handle) {
        return m_AxisList.contains(handle);
    }

    InputAxisHandle InputAxisRepository::AddAxis(std::string_view axis) {
        // hash the axis name for easier identification
        InputAxisHandle handle = engine::FNVConstHash(axis);
        m_AxisList[handle] = axis;
        return handle;
    }
}