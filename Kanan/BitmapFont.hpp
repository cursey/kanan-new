#pragma once

#include <array>

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    class BitmapFont : public Mod {
    public:
        BitmapFont();

        void onPatchUI() override;

        void onConfigLoad(ConfigPtr cfg);
        void onConfigSave(ConfigPtr cfg);

    private:
        bool m_isEnabled;
        std::array<Patch, 4> m_patches;

        void apply();
    };
}
