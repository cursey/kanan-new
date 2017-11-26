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
        Patch m_patchJp;
        Patch m_patchJnz;

        void apply();
    };
}
