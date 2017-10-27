#pragma once

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    class OneClickRevive : public Mod {
    public:
        OneClickRevive();

        void onPatchUI() override;

        void onConfigLoad(ConfigPtr cfg) override;
        void onConfigSave(ConfigPtr cfg) override;

    private:
        bool m_isEnabled;
        Patch m_patch;

        void apply();
    };
}