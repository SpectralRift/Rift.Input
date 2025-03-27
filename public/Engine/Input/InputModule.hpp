#pragma once

namespace engine::input {
    struct InputModule {
        static void ModuleStartup();
        static void ModuleShutdown();
    };
}