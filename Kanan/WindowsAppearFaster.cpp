#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "WindowsAppearFaster.hpp"

using namespace std;

namespace kanan {
    WindowsAppearFaster::WindowsAppearFaster()
        : m_isEnabled{ false },
        m_patches{}
    {
        auto address0 = scan("client.exe", "74 6F E8 ? ? ? ? 85 C0 74 10");
        m_patches[0].address = address0.value_or(0);
        m_patches[0].bytes = { 0xEB };

        auto address1 = scan("client.exe", "8B 4D 18 6A 00 51 8B 8E ? ? ? ?");
        m_patches[1].address = address1.value_or(0);
        m_patches[1].bytes = { 0x31, 0xC9, 0x90 };

        if (address0 && address1) {
            log("Found all addresses for WindowsAppearFaster.");
        }
        else {
            log("Failed to find one or more addresses for WindowsAppearFaster.");
        }
    }

    void WindowsAppearFaster::onPatchUI() {
        for (auto& p : m_patches) {
            if (p.address == 0) {
                return;
            }
        }

        if (ImGui::Checkbox("Windows Appear Faster", &m_isEnabled)) {
            apply();
        }

        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Credits: Step29");
        }
    }

    void WindowsAppearFaster::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("WindowsAppearFaster.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void WindowsAppearFaster::onConfigSave(Config& cfg) {
        cfg.set<bool>("WindowsAppearFaster.Enabled", m_isEnabled);
    }

    void WindowsAppearFaster::apply() {
        for (auto& p : m_patches) {
            if (p.address == 0) {
                return;
            }
        }

        log("Toggling WindowsAppearFaster...");

        if (m_isEnabled) {
            for (auto& p : m_patches) {
                patch(p);
            }
        }
        else {
            for (auto& p : m_patches) {
                undoPatch(p);
            }
        }
    }


}
