#include <imgui.h>

#include "Kanan.hpp"
#include "Log.hpp"
#include "Utility.hpp"
#include <Scan.hpp>

#include "PetSummonGroups.hpp"

namespace kanan {

static PetSummonGroups* g_petSummonGroups;

// Arrays to hold our slot configuration and current state
int32_t summonGroupSlotStart[10];
int32_t summonGoupSlotEnd[10];
int32_t summonGroupSlotCurrent[10];

// hotkeyPressed is the slot index corrosponding to the hotkey number pressed when pressing Summon X Pet hotkey from
// hotkey settings. tabFlag determines which tab of the pet info window is summoned. 0 for Pets, 1 for partners.
void PetSummonGroups::HookForSummoning(int hotkeyPressed, int tabFlag) {
    // Function as normal if our patch isn't enabled
    if (!g_petSummonGroups->m_isEnabled) {
        g_petSummonGroups->m_hook->callOriginal(hotkeyPressed, tabFlag);
        return;
    }
    // Only use summonGroups for pets
    if (tabFlag == 0) {
        // Check to see if we need to set our current slot back to the starting position
        if ((summonGroupSlotCurrent[hotkeyPressed] > summonGoupSlotEnd[hotkeyPressed]) ||
            (summonGroupSlotCurrent[hotkeyPressed] < summonGroupSlotStart[hotkeyPressed])) {
            summonGroupSlotCurrent[hotkeyPressed] = summonGroupSlotStart[hotkeyPressed];
        }
        // Summon the pet from the slot in the pet info window. We subtract 1 from the index number to allow indexing to
        // start at 1 in the UI.
        g_petSummonGroups->m_hook->callOriginal(summonGroupSlotCurrent[hotkeyPressed] - 1, tabFlag);
        summonGroupSlotCurrent[hotkeyPressed]++;
        return;
    } else
        g_petSummonGroups->m_hook->callOriginal(hotkeyPressed, tabFlag);
    return;
}

// Find the addresses we need for patching
PetSummonGroups::PetSummonGroups() : m_hook(nullptr) {
    auto summonAddress = scan("client.exe", "83 FA ? 0F 87 ? ? ? ? 89 54 ? ? 89 4C");
    //  scan("client.exe", "8D 4B ? 85 C9 0F 88 ? ? ? ? 33 D2");Old scan, comes before actual function call with sub to
    //  find summonslot index in the hotkey manager
    if (!summonAddress) {
        log("[Pet Summon Groups] unable to find petSummongroups summonAddress");
    } else {
        log("[Pet Summon Groups] Found petSummongroups summonAddress base @ %p", *summonAddress);
        m_hook = std::make_unique<FunctionHook>(*summonAddress, reinterpret_cast<uintptr_t>(HookForSummoning));
    }
    g_petSummonGroups = this;
}

// Pet Summon Groups UI
void PetSummonGroups::onUI() {
    if (m_hook) {
        if (ImGui::CollapsingHeader("Pet Summon Groups")) {
            ImGui::TextWrapped("Extends the \"Summon X Pet\" hotkeys in hotkey settings into hotkeys with "
                               "configurable summon groups.");
            ImGui::TextWrapped("Tapping a group's hotkey will summon the pets from the Pet Information Window "
                               "sequentially from their slot number");
            ImGui::TextWrapped("Order your pets in order from top to bottom to form groups in the Pet Information "
                               "Window ingame. In the Pet "
                               "Summon groups config enter the start and end slot for summoning pets starting from "
                               "1. Pets will be summoned in their group in order.");
            ImGui::TextWrapped("Use with Pet Desummon Fix to allow desummoning a pet with the hotkeys with one "
                               "already summoned (useful for pet spam).");
            ImGui::Spacing();
            if (ImGui::Checkbox("Enable Pet Summon Groups", &m_isEnabled)) {
            }
            if (m_isEnabled) {
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 1");
                ImGui::InputInt("Summon Group 1 Slot Start", (&summonGroupSlotStart[0]));
                if (summonGroupSlotStart[0] < 1) {
                    summonGroupSlotStart[0] = 1;
                }
                ImGui::InputInt("Summon Group 1 Slot End", (&summonGoupSlotEnd[0]));
                if (summonGoupSlotEnd[0] < 1) {
                    summonGoupSlotEnd[0] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 2");
                ImGui::InputInt("Summon Group 2 Slot Start", (&summonGroupSlotStart[1]));
                if (summonGroupSlotStart[1] < 1) {
                    summonGroupSlotStart[1] = 1;
                }
                ImGui::InputInt("Summon Group 2 Slot End", (&summonGoupSlotEnd[1]));
                if (summonGoupSlotEnd[1] < 1) {
                    summonGoupSlotEnd[1] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 3");
                ImGui::InputInt("Summon Group 3 Slot Start", (&summonGroupSlotStart[2]));
                if (summonGroupSlotStart[2] < 1) {
                    summonGroupSlotStart[2] = 1;
                }
                ImGui::InputInt("Summon Group 3 Slot End", (&summonGoupSlotEnd[2]));
                if (summonGoupSlotEnd[2] < 1) {
                    summonGoupSlotEnd[2] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 4");
                ImGui::InputInt("Summon Group 4 Slot Start", (&summonGroupSlotStart[3]));
                if (summonGroupSlotStart[3] < 1) {
                    summonGroupSlotStart[3] = 1;
                }
                ImGui::InputInt("Summon Group 4 Slot End", (&summonGoupSlotEnd[3]));
                if (summonGoupSlotEnd[3] < 1) {
                    summonGoupSlotEnd[3] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 5");
                ImGui::InputInt("Summon Group 5 Slot Start", (&summonGroupSlotStart[4]));
                if (summonGroupSlotStart[4] < 1) {
                    summonGroupSlotStart[4] = 1;
                }
                ImGui::InputInt("Summon Group 5 Slot End", (&summonGoupSlotEnd[4]));
                if (summonGoupSlotEnd[4] < 1) {
                    summonGoupSlotEnd[4] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 6");
                ImGui::InputInt("Summon Group 6 Slot Start", (&summonGroupSlotStart[5]));
                if (summonGroupSlotStart[5] < 1) {
                    summonGroupSlotStart[5] = 1;
                }
                ImGui::InputInt("Summon Group 6 Slot End", (&summonGoupSlotEnd[5]));
                if (summonGoupSlotEnd[5] < 1) {
                    summonGoupSlotEnd[5] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 7");
                ImGui::InputInt("Summon Group 7 Slot Start", (&summonGroupSlotStart[6]));
                if (summonGroupSlotStart[6] < 1) {
                    summonGroupSlotStart[6] = 1;
                }
                ImGui::InputInt("Summon Group 7 Slot End", (&summonGoupSlotEnd[6]));
                if (summonGoupSlotEnd[6] < 1) {
                    summonGoupSlotEnd[6] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 8");
                ImGui::InputInt("Summon Group 8 Slot Start", (&summonGroupSlotStart[7]));
                if (summonGroupSlotStart[7] < 1) {
                    summonGroupSlotStart[7] = 1;
                }
                ImGui::InputInt("Summon Group 8 Slot End", (&summonGoupSlotEnd[7]));
                if (summonGoupSlotEnd[7] < 1) {
                    summonGoupSlotEnd[7] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 9");
                ImGui::InputInt("Summon Group 9 Slot Start", (&summonGroupSlotStart[8]));
                if (summonGroupSlotStart[8] < 1) {
                    summonGroupSlotStart[8] = 1;
                }
                ImGui::InputInt("Summon Group 9 Slot End", (&summonGoupSlotEnd[8]));
                if (summonGoupSlotEnd[8] < 1) {
                    summonGoupSlotEnd[8] = 1;
                }
                ImGui::Spacing();
                ImGui::Text("Pet Summon Group 10");
                ImGui::InputInt("Summon Group 10 Slot Start", (&summonGroupSlotStart[9]));
                if (summonGroupSlotStart[9] < 1) {
                    summonGroupSlotStart[9] = 1;
                }
                ImGui::InputInt("Summon Group 10 Slot End", (&summonGoupSlotEnd[9]));
                if (summonGoupSlotEnd[9] < 1) {
                    summonGoupSlotEnd[9] = 1;
                }
            }
        }
    }
}

// Pet Summon Groups Config saving and loading

void PetSummonGroups::onConfigSave(Config& cfg) {
    cfg.set<bool>("PetSummonGroups.Enabled", m_isEnabled);
    for (int groupSlot = 0; groupSlot < 10; groupSlot++) {
        auto slotString = std::to_string(groupSlot);
        cfg.set<int>(("PetSummonGroups.summonGroupSlotStart" + slotString).c_str(), summonGroupSlotStart[groupSlot]);
        cfg.set<int>(("PetSummonGroups.summonGroupSlotEnd" + slotString).c_str(), summonGoupSlotEnd[groupSlot]);
    }
}
void PetSummonGroups::onConfigLoad(const Config& cfg) {
    m_isEnabled = cfg.get<bool>("PetSummonGroups.Enabled").value_or(false);
    for (int groupSlot = 0; groupSlot < 10; groupSlot++) {
        auto slotString = std::to_string(groupSlot);
        summonGroupSlotStart[groupSlot] =
            cfg.get<int>(("PetSummonGroups.summonGroupSlotStart" + slotString).c_str()).value_or(groupSlot + 1);
        summonGoupSlotEnd[groupSlot] =
            cfg.get<int>(("PetSummonGroups.summonGroupSlotEnd" + slotString).c_str()).value_or(groupSlot + 1);
    }
}

} // namespace kanan