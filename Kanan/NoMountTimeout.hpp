#pragma once

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    class NoMountTimeout : public Mod {
    public:
        NoMountTimeout();

        void onPatchUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        Patch m_patch;

        void apply();
    };
}
