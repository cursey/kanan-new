#pragma once

#include <array>

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    class WindowsAppearFaster : public Mod {
    public:
        WindowsAppearFaster();

        void onPatchUI() override;

        void onConfigLoad(ConfigPtr cfg) override;
        void onConfigSave(ConfigPtr cfg) override;

    private:
        bool m_isEnabled;
        std::array<Patch, 2> m_patches;

        void apply();
    };
}