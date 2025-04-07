cmake_minimum_required(VERSION 3.14)
project(Rift_Input VERSION 0.1.1)

add_library(
        Rift_Input
        STATIC
        private/Engine/Input/InputModule.cpp
        private/Engine/Input/InputManager.cpp
        private/Engine/Input/InputSystem.cpp
        private/Engine/Input/InputKeyRepository.cpp
        private/Engine/Input/InputAxisRepository.cpp
)

target_include_directories(
        Rift_Input
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private"
)

rift_resolve_module_libs("Rift.Core.Runtime" RIFT_INPUT_DEPS)

target_link_libraries(Rift_Input ${RIFT_INPUT_DEPS})