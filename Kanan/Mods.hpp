#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Mod.hpp"

namespace kanan {
    class Mods {
    public:
        Mods(const std::string& patchesFilepath);

        const auto& getMods() const {
            return m_mods;
        }

    private:
        std::vector<std::unique_ptr<Mod>> m_mods;
    };
}
