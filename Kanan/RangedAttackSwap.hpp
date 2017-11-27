#pragma once

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    class RangedAttackSwap : public Mod {
    public:
        RangedAttackSwap();

        void onPatchUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        Patch m_patch;
        int m_choice;

        void apply();
    };
}
