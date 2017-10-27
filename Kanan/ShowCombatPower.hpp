#pragma once

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    class ShowCombatPower : public Mod {
    public:
        ShowCombatPower();

        void onPatchUI() override;

        void onConfigLoad(ConfigPtr cfg);
        void onConfigSave(ConfigPtr cfg);

    private:
        bool m_isEnabled;
        Patch m_patch;

        void apply();
    };
}
