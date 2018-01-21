#pragma once

#include <Patch.hpp>

#include "PatchMod.hpp"

namespace kanan {
    class RangedAttackSwap : public PatchMod {
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
