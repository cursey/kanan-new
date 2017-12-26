#include <fstream>
#include <filesystem>

#include <json.hpp>
#include <String.hpp>

#include "PatchMod.hpp"
#include "RangedAttackSwap.hpp"
#include "AutoSetMTU.hpp"
#include "DisableNagle.hpp"
#include "BorderlessWindow.hpp"
#include "EnableMultiClient.hpp"
#include "EntityViewer.hpp"
#include "ColorAltText.hpp"
#include "ZeroFogDistance.hpp"

#include "Log.hpp"

#include "Mods.hpp"

using namespace std;
using namespace std::experimental::filesystem;
using nlohmann::json;

namespace kanan {
    Mods::Mods(const std::string& filepath)
        : m_mods{},
        m_patchMods{}
    {
        log("Entering Mods cosntructor.");

        // Load all .json files.
        for (const auto& p : directory_iterator(filepath)) {
            auto& path = p.path();

            if (path.extension() != ".json") {
                continue;
            }

            // Load patches from the patches json file.
            // HACK: Use microsoft's wstring constructor even though its not standard
            // because utf8 filepaths aren't valid in windows.
            ifstream patchesFile{ path };

            if (!patchesFile) {
                log("Failed to load patches file: %s", path.c_str());
            }

            json patches{};

            patchesFile >> patches;

            for (auto& patch : patches) {
                // Create the new PatchMod
                auto patchMod = make_unique<PatchMod>();

                // Load it from json.
                *patchMod = patch;

                m_patchMods[patchMod->getCategory()].emplace_back(move(patchMod));
            }
        }

		m_mods.emplace_back(make_unique<ZeroFogDistance>());
        m_mods.emplace_back(make_unique<RangedAttackSwap>());
		m_mods.emplace_back(make_unique<ColorAltText>());

        m_mods.emplace_back(make_unique<AutoSetMTU>());
        m_mods.emplace_back(make_unique<DisableNagle>());
        m_mods.emplace_back(make_unique<BorderlessWindow>());
        m_mods.emplace_back(make_unique<EnableMultiClient>());
        m_mods.emplace_back(make_unique<EntityViewer>());

        log("Leaving Mods constructor.");
    }
}
