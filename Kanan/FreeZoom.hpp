#pragma once

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    class FreeZoom : public Mod {
    public:
        FreeZoom();

        void onPatchUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        Patch m_patch_jp;
        Patch m_patch_jnz;

        void apply();
    };
}
