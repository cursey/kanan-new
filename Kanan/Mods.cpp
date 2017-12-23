#include <fstream>

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

#include "Log.hpp"

#include "Mods.hpp"

using namespace std;
using nlohmann::json;

namespace kanan {
    Mods::Mods(const std::string& patchesFilepath)
        : m_mods{}
    {
        log("Entering Mods cosntructor.");

        // Load patches from the patches json file.
        // HACK: Use microsoft's wstring constructor even though its not standard
        // because utf8 filepaths aren't valid in windows.
        ifstream patchesFile{ widen(patchesFilepath) };

        if (!patchesFile) {
            log("Failed to load patches file: %s", patchesFilepath.c_str());
        }

        json patches{};

        patchesFile >> patches;

        for (auto& patch : patches) {
            auto patchMod = make_unique<PatchMod>();
            *patchMod = patch;
            m_mods.emplace_back(move(patchMod));
        }

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
