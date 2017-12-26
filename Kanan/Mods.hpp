#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "Mod.hpp"
#include "PatchMod.hpp"

namespace kanan {
    class Mods {
    public:
        Mods(const std::string& filepath);

        const auto& getMods() const {
            return m_mods;
        }

        const auto& getPatchMods() const {
            return m_patchMods;
        }

    private:
        std::vector<std::unique_ptr<Mod>> m_mods;
        std::map<std::string, std::vector<std::unique_ptr<PatchMod>>> m_patchMods;
    };
}
