#include <fstream>
#include <filesystem>
#include <thread>

#include <json.hpp>
#include <String.hpp>
#include <Config.hpp>

#include "PatchMod.hpp"
#include "RangedAttackSwap.hpp"
#include "AutoSetMTU.hpp"
#include "DisableNagle.hpp"
#include "BorderlessWindow.hpp"
#include "EnableMultiClient.hpp"
#include "EntityViewer.hpp"
#include "ColorAltText.hpp"
#include "EquipmentOverride.hpp"
#include "FieldOfView.hpp"
#include "UseDataFolder.hpp"
#include "FreezeTimeOfDay.hpp"

#include "Log.hpp"

#include "Mods.hpp"

using namespace std;
using namespace std::experimental::filesystem;
using nlohmann::json;

namespace kanan {
    Mods::Mods(std::string filepath)
        : m_filepath{ move(filepath) },
        m_mods{},
        m_patchMods{}
    {
        log("[Mods] Entering cosntructor.");
        log("[Mods] Leaving constructor.");
    }

    void Mods::loadTimeCriticalMods() {
        log("[Mods] Loading time critical mods...");

        m_mods.emplace_back(make_unique<UseDataFolder>());

        // Time critical mods need to have their settings loaded from the config
        // right away.
        Config cfg{ m_filepath + "/config.txt" };

        for (auto& mod : m_mods) {
            mod->onConfigLoad(cfg);
        }

        log("[Mods] Finished loading time critical mods.");

        m_areTimeCriticalModsLoaded = true;
    }

    void Mods::loadMods() {
        while (!m_areTimeCriticalModsLoaded) {
            this_thread::yield();
        }

        log("[Mods] Loading mods...");

        // Load all .json files.
        for (const auto& p : directory_iterator(m_filepath)) {
            auto& path = p.path();

            if (path.extension() != ".json") {
                continue;
            }

            // Load patches from the patches json file.
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

        m_patchMods["Quality of Life"].emplace_back(make_unique<RangedAttackSwap>());
		m_patchMods["Text"].emplace_back(make_unique<ColorAltText>());

        for (auto& categories : m_patchMods) {
            auto& mods = categories.second;

            sort(mods.begin(), mods.end(), [](const auto& a, const auto& b) {
                return a->getName() < b->getName();
            });
        }

        m_mods.emplace_back(make_unique<AutoSetMTU>());
        m_mods.emplace_back(make_unique<DisableNagle>());
        m_mods.emplace_back(make_unique<BorderlessWindow>());
        m_mods.emplace_back(make_unique<EnableMultiClient>());
        m_mods.emplace_back(make_unique<EntityViewer>());
        m_mods.emplace_back(make_unique<EquipmentOverride>());
        m_mods.emplace_back(make_unique<FieldOfView>());
        m_mods.emplace_back(make_unique<FreezeTimeOfDay>());

        log("[Mods] Finished loading mods.");
    }
}
