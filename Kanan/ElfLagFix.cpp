#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "ElfLagFix.hpp"

using namespace std;

namespace kanan {
    ElfLagFix::ElfLagFix()
        : m_isEnabled{ false },
        m_patch{}
    {
        auto address = scan("client.exe", "55 8B EC 56 57 8B F9 8B 07 8B 50 0C");

        if (address) {
            g_log << "Got ElfLagFix " << hex << *address << endl;

            m_patch.address = *address;
            m_patch.bytes = { 0x32, 0xC0, 0xC2, 0x04, 0x00 };
        }
        else {
            g_log << "Failed to find ElfLagFix address" << endl;
        }
    }

    void ElfLagFix::onPatchUI() {
        if (m_patch.address == 0) {
            return;
        }

        if (ImGui::Checkbox("Elf Lag Fix", &m_isEnabled)) {
            apply();
        }
    }

    void ElfLagFix::onConfigLoad(ConfigPtr cfg) {
        m_isEnabled = cfg->get_qualified_as<bool>("ElfLagFix.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void ElfLagFix::onConfigSave(ConfigPtr cfg) {
        auto tbl = cpptoml::make_table();

        tbl->insert("Enabled", m_isEnabled);
        cfg->insert("ElfLagFix", tbl);
    }

    void ElfLagFix::apply() {
        if (m_patch.address == 0) {
            return;
        }

        g_log << "Toggling ElfLagFix..." << endl;

        if (m_isEnabled) {
            patch(m_patch);
        }
        else {
            undoPatch(m_patch);
        }
    }
}

