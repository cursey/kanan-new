#pragma once

#include <string>
#include <vector>

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    // Easier to use Mod that works for any mods that are just simple toggleable patches.
    class PatchMod : public Mod {
    public:
        PatchMod(std::string patchName, std::string tooltip);

        void onPatchUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    protected:
        bool addPatch(const std::string& pattern, int offset, std::vector<int16_t> patchBytes);

    private:
        bool m_isEnabled;
        std::vector<Patch> m_patches;
        std::string m_patchName;
        std::string m_tooltip;
        std::string m_configName;

        void apply();
    };
}
