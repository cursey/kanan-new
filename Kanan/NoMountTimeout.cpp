#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "NoMountTimeout.hpp"

using namespace std;

namespace kanan {
    NoMountTimeout::NoMountTimeout()
        : m_isEnabled{ false },
        m_patch{}
    {
        log("Entering NoMountTimeout.");

        auto address = scan("client.exe", "89 90 58 02 00 00");

        if (address) {
            log("Got NoMountTimeout %p", *address);

            m_patch.address = *address;
            m_patch.bytes = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
        }
        else {
            log("Failed to find NoMountTimeout address.");
        }

        log("Leaving NoMountTimeout.");
    }

    void NoMountTimeout::onPatchUI() {
        if (m_patch.address == 0) {
            return;
        }

        if (ImGui::Checkbox("No Mount Timeout", &m_isEnabled)) {
            apply();
        }

        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Credits: Licat");
        }
    }

    void NoMountTimeout::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("NoMountTimeout.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void NoMountTimeout::onConfigSave(Config& cfg) {
        cfg.set<bool>("NoMountTimeout.Enabled", m_isEnabled);
    }

    void NoMountTimeout::apply() {
        if (m_patch.address == 0) {
            return;
        }

        log("Toggling NoMountTimeout...");

        if (m_isEnabled) {
            patch(m_patch);
        }
        else {
            undoPatch(m_patch);
        }
    }
}
