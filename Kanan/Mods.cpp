#include <fstream>
#include <filesystem>
#include <thread>

#include <nlohmann/json.hpp>
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
#include "TTFFontSize.hpp"
#include "StatusUI.hpp"
#include "LoginScreen.hpp"
#include "DontMoveToSquadChat.hpp"
#include "AutoChangeChannels.hpp"
#include "ChangeChannelHotkey.hpp"
#include "Currtarget.hpp"
#include "CookingMod.hpp"
#include "CharacterWindowTitle.hpp"
#include "DrawDistance.hpp"
#include "PetSummonGroups.hpp"
#include "PetDesummonFix.hpp"

#include "Log.hpp"

#include "Mods.hpp"

using namespace std;
using namespace std::filesystem;
using nlohmann::json;

namespace kanan {
    Mods::Mods(std::string filepath)
        : m_filepath{ move(filepath) },
        m_mods{},
        m_patchMods{},
        m_modsMutex{}
    {
        log("[Mods] Entering cosntructor.");
        log("[Mods] Leaving constructor.");
    }

    void Mods::loadTimeCriticalMods() {
        log("[Mods] Loading time critical mods...");

        addMod(make_unique<UseDataFolder>());
        addMod(make_unique<LoginScreen>());

        // Time critical mods need to have their settings loaded from the config
        // right away.
        Config cfg{ m_filepath + "/config.txt" };

        for (auto& mod : m_mods) {
            mod->onConfigLoad(cfg);
        }

        log("[Mods] Finished loading time critical mods.");
    }

    void Mods::loadMods() {
        log("[Mods] Loading mods...");

        // Load all .json files.
        for (const auto& p : directory_iterator(m_filepath)) {
            auto& path = p.path();

            if (path.extension() != ".json") {
                continue;
            }

            auto filename = path.filename().string();

            std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

            if (filename.find("patch") == std::string::npos) {
                continue;
            }

            // Load patches from the patches json file.
            ifstream patchesFile{ path };

            if (!patchesFile) {
                log("Failed to load patches file: %s", path.string().c_str());
            }

            json patches{};

            patchesFile >> patches;

            for (auto& patch : patches) {
                // Create the new PatchMod
                auto patchMod = make_unique<PatchMod>();

                // Load it from json.
                *patchMod = patch;

                addPatchMod(patchMod->getCategory(), move(patchMod));
            }
        }

        addPatchMod("Quality of Life", make_unique<RangedAttackSwap>());
        addPatchMod("Speedup", make_unique<DontMoveToSquadChat>());
		addPatchMod("Text", make_unique<TTFFontSize>());
        addPatchMod("Text", make_unique<ColorAltText>());
        addPatchMod("Fixes", make_unique<PetDesummonFix>());

        for (auto& categories : m_patchMods) {
            auto& mods = categories.second;

            sort(mods.begin(), mods.end(), [](const auto& a, const auto& b) {
                return a->getName() < b->getName();
            });
        }

        addMod(make_unique<AutoSetMTU>());
        addMod(make_unique<DisableNagle>());
        addMod(make_unique<BorderlessWindow>());
        addMod(make_unique<EnableMultiClient>());
        addMod(make_unique<EntityViewer>());
        addMod(make_unique<CookingMod>());
        addMod(make_unique<EquipmentOverride>());
        addMod(make_unique<FieldOfView>());
        addMod(make_unique<DrawDistance>());
        addMod(make_unique<FreezeTimeOfDay>());
        /* addMod(make_unique<StatusUI>());
        addMod(make_unique<AutoChangeChannels>());
        addMod(make_unique<ChangeChannelHotkey>());*/
        addMod(make_unique<Currtarget>());
        addMod(make_unique<CharacterWindowTitle>());
        addMod(make_unique<PetSummonGroups>());

        

        log("[Mods] Finished loading mods.");
    }

    void Mods::addMod(std::unique_ptr<Mod>&& mod) {
        scoped_lock<mutex> _{ m_modsMutex };

        m_mods.emplace_back(move(mod));
    }

    void Mods::addPatchMod(const std::string& category, std::unique_ptr<PatchMod>&& mod) {
        scoped_lock<mutex> _{ m_modsMutex };

        m_patchMods[category].emplace_back(move(mod));
    }
}
