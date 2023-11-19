#include <imgui.h>

#include "Kanan.hpp"
#include "Log.hpp"
#include "Utility.hpp"
#include <Scan.hpp>

#include "Mabinogi.hpp"
#include "PetDesummonFix.hpp"

namespace kanan {

static PetDesummonFix* g_petHotkeyManager;

// get our addresses
PetDesummonFix::PetDesummonFix()
    : PatchMod{"Pet Desummon Fix",
          "Alters the Summon X Pet hotkeys in hotkey settings to desummon the currently summoned pet, if a pet is "
          "summoned, to match behavior of hotbar hotkeys containing pets."},
      m_petHotkeyHook(nullptr) {
    auto petHotkeyManagerAddress =
        scan("client.exe", "48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 48 8B ? ? ? ? ? 48 8D ? ? ? ? ? 8B FA");
    //  scan("client.exe", "8D 4B ? 85 C9 0F 88 ? ? ? ? 33 D2");Old scan, comes before actual function call with sub to
    //  find summonslot index in the hotkey manager
    if (!petHotkeyManagerAddress) {
        log("[Pet Desummon Fix] unable to find petHotkeyManagerAddress");
    } else {
        log("[Pet Desummon Fix] Found petHotkeyManagerAddress base @ %p", *petHotkeyManagerAddress);
        m_petHotkeyHook =
            std::make_unique<FunctionHook>(*petHotkeyManagerAddress, reinterpret_cast<uintptr_t>(PetHotkeyHook));
    }
    g_petHotkeyManager = this;

    auto HotkeyHookParam1 = scan("client.exe", "48 8B ? ? ? ? ? E8 ? ? ? ? B0 ? 48 8B ? ? ? 48 83 C4 ? 5F 5E 5D C3 48 8B ? E8 ? ? ? ? 84 C0");

    if (HotkeyHookParam1) {
        do {
            m_petHotkeyHookParam1 = (HANDLE*)rel_to_abs(*HotkeyHookParam1 + 3);
        } while (*m_petHotkeyHookParam1 == nullptr);

        log("[Pet Desummon Fix] Found HotkeyHookParam1 %p", *m_petHotkeyHookParam1);
    } else {
        log("[Pet Desummon Fix] Failed to find HotkeyHookParam1.");
    }
}

void PetDesummonFix::PetHotkeyHook(LONGLONG param1, INT param2) {

    if (!g_petHotkeyManager->m_isEnabled) {
        g_petHotkeyManager->m_petHotkeyHook->callOriginal(param1, param2);
        return;
    } else {
        auto game = g_kanan->getGame();
        auto localCharacter = game->getLocalCharacter();
        auto summonMaster = localCharacter->summonMaster;
        // Check to see if a summon pet hotkey was pressed.
        // param 2 handles the ID of the hotkey pressed.
        // We check to see if we are using a summon hotkey - if so we have modified code to run.
        switch (param2) {
        case 0x6C:// Summon pet in slot 1 hotkey - rest are just 2 through 10
        case 0x6D:
        case 0x6E:
        case 0x6F:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75: // summon pet in slot 10 hotkey
            // If we don't have summonMaster we log error and procede to function without the fix.
            if (!summonMaster) {
                g_petHotkeyManager->m_petHotkeyHook->callOriginal(param1, param2);
                log("[Pet Desummon Fix] Failed to summonMaster");
                return;
            }
            // If our slaveEntityID is not 0 we have a pet summoned so we'll desummon it.
            if (!summonMaster->slaveEntityID == 0x0000000000000000) {
                // desummon the pet
                g_petHotkeyManager->m_petHotkeyHook->callOriginal(param1, 0x7B);//7B is the param for the desummon hotkey press
                break;
            }
            g_petHotkeyManager->m_petHotkeyHook->callOriginal(param1, param2);
            break;
        default:
            g_petHotkeyManager->m_petHotkeyHook->callOriginal(param1, param2);
            break;
        }
        return;
    }
}

void PetDesummonFix::onPatchUI() {
    if (m_petHotkeyHook) {
        if (ImGui::Checkbox("Enable Pet Desummon Fix", &m_isEnabled)) {
        }
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Alters the Summon X Pet hotkeys in hotkey settings to match hotbar functionality.\n"
                          "If a pet is summoned when the hotkey is pressed, the pet will be desummoned.");
    }
}

void PetDesummonFix::onConfigLoad(const Config& cfg) {
    m_isEnabled = cfg.get<bool>("PetDesummonFix.Enabled").value_or(false);
}

void PetDesummonFix::onConfigSave(Config& cfg) {
    cfg.set<bool>("PetDesummonFix.Enabled", m_isEnabled);
}
} // namespace kanan
