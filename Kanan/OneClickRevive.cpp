#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "OneClickRevive.hpp"

using namespace std;

namespace kanan {
    OneClickRevive::OneClickRevive()
        : m_isEnabled{ false },
        m_patch{}
    {
        auto address = scan("client.exe", "0F 86 ? ? ? ? 8B 4D E8 8B 11");

        if (address) {
            log("Got OneClickRevive %p", *address);

            m_patch.address = *address;
            m_patch.bytes = { 0x90, 0xE9 };
        }
        else {
            log("Failed to find OneClickRevive address.");
        }
    }

    void OneClickRevive::onPatchUI() {
        if (m_patch.address == 0) {
            return;
        }

        if (ImGui::Checkbox("One Click Revive", &m_isEnabled)) {
            apply();
        }
    }

    void OneClickRevive::onConfigLoad(ConfigPtr cfg) {
        m_isEnabled = cfg->get_qualified_as<bool>("OneClickRevive.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void OneClickRevive::onConfigSave(ConfigPtr cfg) {
        auto tbl = cpptoml::make_table();

        tbl->insert("Enabled", m_isEnabled);
        cfg->insert("OneClickRevive", tbl);
    }

    void OneClickRevive::apply() {
        if (m_patch.address == 0) {
            return;
        }

        log("Toggling OneClickRevive...");

        if (m_isEnabled) {
            patch(m_patch);
        }
        else {
            undoPatch(m_patch);
        }
    }
}