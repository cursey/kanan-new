#pragma once

#include <cstdint>

#include "Mod.hpp"

namespace kanan {
    class UseDataFolder : public Mod {
    public:
        UseDataFolder();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        uintptr_t m_fileSystem;
        int(__thiscall* m_setLookUpOrder)(uintptr_t fileSystem, int a2);

        void apply();
    };
}
