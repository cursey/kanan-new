#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "RangedAttackSwap.hpp"

using namespace std;

namespace kanan {
    RangedAttackSwap::RangedAttackSwap()
        : m_choice{0},
        m_applied_choice{0},
        m_patch{}
    {
        auto address = scan("client.exe", "B8 09 52 00 00 5F");

        if (address) {
            log("Got RangedAttackSwap %p", *address);

            m_patch.address = *address + 1;
            m_patch.bytes = { 0x09, 0x52 };
        }
        else {
            log("Failed to find RangedAttackSwap address.");
        }
    }

    void RangedAttackSwap::onUI() {
        if (m_patch.address == 0) {
            return;
        }
        if (ImGui::CollapsingHeader("Ranged Attack Swap")) {
            ImGui::Combo("Skill", &m_choice, "Disabled\0Magnum Shot\0Arrow Revolver\0Support Shot\0Mirage Missile\0Crash Shot\0Spider Shot\0Urgent Shot\0\0");
            if (m_choice != m_applied_choice) {
                apply();
            }
        }
    }

    void RangedAttackSwap::onConfigLoad(const Config& cfg) {
        m_choice = cfg.get<int>("RangedAttackSwap.Choice").value_or(0);

        if (m_choice != 0) {
            apply();
        }
    }

    void RangedAttackSwap::onConfigSave(Config& cfg) {
        cfg.set<int>("RangedAttackSwap.Choice", m_choice);
    }

    void RangedAttackSwap::apply() {
        if (m_patch.address == 0) {
            return;
        }
        log("Applying RangedAttackSwap...");
        switch (m_choice) {
        case 1: //Magnum Shot
            m_patch.bytes = { 0x0A, 0x52 };
            break;
        case 2: //Arrow Revolver
            m_patch.bytes = { 0x0C, 0x52 };
            break;
        case 3: //Support Shot
            m_patch.bytes = { 0x0E, 0x52 };
            break;
        case 4: //Mirage Missile
            m_patch.bytes = { 0x0F, 0x52 };
            break;
        case 5: //Crash Shot
            m_patch.bytes = { 0xFB, 0x55 };
            break;
        case 6: //Spider Shot
            m_patch.bytes = { 0x14, 0x52 };
            break;
        case 7: //Urgent Shot
            m_patch.bytes = { 0x16, 0x52 };
            break;
        default: //Disable on default
        case 0: //Disabled
            m_patch.bytes = { 0x09, 0x52 };
        }
        patch(m_patch);
        m_applied_choice = m_choice;
    }
}
