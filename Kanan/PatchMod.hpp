#pragma once

#include <string>
#include <vector>

#include <json.hpp>
#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
    // Easier to use Mod that works for any mods that are just simple toggleable patches.
    class PatchMod final : public Mod {
    public:
        PatchMod() = default;
        PatchMod(std::string patchName, std::string tooltip);

        void onPatchUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

        const auto& getCategory() {
            return m_category;
        }

        void setCategory(std::string category) {
            m_category = move(category);
        }

    protected:
        bool addPatch(const std::string& pattern, int offset, std::vector<int16_t> patchBytes, int n = 0);
        void buildConfigName();

    private:
        bool m_isEnabled;
        std::vector<Patch> m_patches;
        bool m_hasFailingPatch;
        std::string m_patchName;
        std::string m_tooltip;
        std::string m_configName;
        std::string m_category;

        void apply();

        friend void from_json(const nlohmann::json& j, PatchMod& mod);
    };

    void from_json(const nlohmann::json& j, PatchMod& mod);
}
